"""Run clang-tidy over a generated translation unit and report the real one.

module_rewriter.py turns a C++20 module source into an ordinary translation
unit under bazel-out. clang-tidy then reports its findings against that
generated path, which is not a file anybody edits -- and it ignores `#line`
directives when it prints a diagnostic, so the generated file cannot simply
claim to be the original. This driver closes the gap: it maps every generated
path in clang-tidy's output back to the source path it came from. The rewrite
is line- and column-preserving, so the positions need no adjustment.

Apart from that mapping it behaves like rules_lint's clang_tidy_wrapper.bash:
the same summary lines are filtered out of the report, the report is written to
--out, and either the exit code is written to --exit-code-file (and this
process succeeds) or the diagnostics are echoed to stderr and the exit code is
propagated, which is what `--@aspect_rules_lint//lint:fail_on_violation` wants.
"""

import argparse
import re
import subprocess
import sys

# Summary chatter clang-tidy prints even on a clean run. rules_lint filters the
# same set; leaving it in would make an empty report look like a finding.
_NOISE_RE = re.compile(
    r"^(?:[0-9]+ (?:warnings?|errors?)(?: and [0-9]+ errors?)? generated\.|"
    r"Suppressed [0-9]+ warnings? \(.*\)\.|"
    r"Use -header-filter=.*|"
    r"[0-9]+ warnings? treated as errors?)$"
)


def _path_pattern(exec_path):
    """A regex matching however clang-tidy chose to spell `exec_path`.

    clang-tidy prints absolute, native-separator paths, so match an optional
    leading directory prefix and accept either separator.
    """
    parts = [re.escape(part) for part in exec_path.replace("\\", "/").split("/")]
    return re.compile(r"(?:[A-Za-z]:)?[^\s:]*?" + r"[\\/]".join(parts))


def main(argv):
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--clang-tidy", required=True)
    parser.add_argument("--out", required=True, help="file to write the report to")
    parser.add_argument("--exit-code-file")
    parser.add_argument(
        "--map",
        action="append",
        default=[],
        dest="maps",
        help="<generated path>=<path to report it as>, repeatable",
    )
    parser.add_argument(
        "--strip-prefix",
        action="append",
        default=[],
        dest="prefixes",
        help="directory holding rewritten copies of repository files at their "
        "own relative paths; it is removed from every path in the report",
    )
    parser.add_argument("command", nargs=argparse.REMAINDER)
    args = parser.parse_args(argv)

    command = args.command
    if command and command[0] == "--":
        command = command[1:]

    result = subprocess.run(
        [args.clang_tidy] + command,
        # Bazel's copy of clang-tidy has no file extension, and Windows'
        # CreateProcess appends ".exe" to a command line that lacks one unless
        # the program is named separately from its arguments.
        executable=args.clang_tidy,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        universal_newlines=True,
    )

    # Explicit maps first: a module interface's copy carries an extra
    # extension that a bare prefix strip would leave behind.
    substitutions = []
    for mapping in args.maps:
        generated, _, original = mapping.partition("=")
        substitutions.append((_path_pattern(generated), original))
    for prefix in args.prefixes:
        substitutions.append((re.compile(_path_pattern(prefix).pattern + r"[\\/]"), ""))

    report = []
    for line in (result.stdout or "").splitlines():
        if _NOISE_RE.match(line.rstrip()):
            continue
        for pattern, original in substitutions:
            line = pattern.sub(original, line)
        report.append(line)

    text = "\n".join(report)
    if text:
        text += "\n"
    with open(args.out, "w", encoding="utf-8", newline="\n") as handle:
        handle.write(text)

    if args.exit_code_file:
        with open(args.exit_code_file, "w", encoding="utf-8", newline="\n") as handle:
            handle.write("%d\n" % result.returncode)
        return 0

    if result.returncode != 0 and text:
        # fail_on_violation mode: Bazel only shows the action's own output, so
        # the diagnostics have to reach stderr or the failure has no body.
        sys.stderr.write(text)
    return result.returncode


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
