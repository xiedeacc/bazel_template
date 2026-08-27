import argparse
from builtins import bytes  # pylint: disable=redefined-builtin
import json
import os
import shutil
import subprocess


def parse_branch_ref(filename):
    try:
        if not os.path.exists(filename):
            print("Warning: HEAD file does not exist: %s" % filename)
            return None
        data = open(filename).read().strip()
        if not data:
            print("Warning: HEAD file is empty: %s" % filename)
            return None
        items = data.split(" ")
        if len(items) == 1:
            return None
        elif len(items) == 2 and items[0] == "ref:":
            return items[1].strip()
        else:
            print("Warning: Git directory has unparseable HEAD, using None: %s" % data)
            return None
    except Exception as e:
        print("Warning: Failed to parse branch ref from %s: %s" %
              (filename, str(e)))
        return None


def configure(src_base_path, gen_path, debug=False):
    git_path = os.path.join(src_base_path, ".git")
    if os.path.exists(gen_path):
        if os.path.isdir(gen_path):
            try:
                shutil.rmtree(gen_path)
            except OSError:
                raise RuntimeError(
                    "Cannot delete directory %s due to permission "
                    "error, inspect and remove manually" % gen_path)
        else:
            raise RuntimeError("Cannot delete non-directory %s, inspect ",
                               "and remove manually" % gen_path)
    os.makedirs(gen_path)

    if not os.path.isdir(gen_path):
        raise RuntimeError("gen_git_source.py: Failed to create dir")

    spec = {}

    link_map = {"head": None, "branch_ref": None}

    if not os.path.isdir(git_path):
        spec["git"] = False
        open(os.path.join(gen_path, "head"), "w").write("")
        open(os.path.join(gen_path, "branch_ref"), "w").write("")
    else:
        spec["git"] = True
        spec["path"] = src_base_path
        git_head_path = os.path.join(git_path, "HEAD")
        spec["branch"] = parse_branch_ref(git_head_path)
        link_map["head"] = git_head_path
        if spec["branch"] is not None:
            # attached method
            link_map["branch_ref"] = os.path.join(
                git_path, *os.path.split(spec["branch"]))

    for target, src in link_map.items():
        if src is None:
            open(os.path.join(gen_path, target), "w").write("")
        elif not os.path.exists(src):
            open(os.path.join(gen_path, target), "w").write("")
            spec["git"] = False
        else:
            try:
                if hasattr(os, "symlink"):
                    os.symlink(src, os.path.join(gen_path, target))
                else:
                    shutil.copy2(src, os.path.join(gen_path, target))
            except OSError:
                shutil.copy2(src, os.path.join(gen_path, target))

    json.dump(spec, open(os.path.join(gen_path, "spec.json"), "w"), indent=2)
    if debug:
        print("gen_git_source.py: list %s" % gen_path)
        print("gen_git_source.py: %s" + repr(os.listdir(gen_path)))
        print("gen_git_source.py: spec is %r" % spec)


def get_git_version(git_base_path):
    unknown_label = b"unknown"
    try:
        print("git_base_path: " + git_base_path)
        # First try to get the commit hash
        val = bytes(
            subprocess.check_output([
                "git",
                str("--git-dir=%s.git" % git_base_path),
                str("--work-tree=%s" % git_base_path), "rev-parse", "HEAD"
            ]).strip())

        # `git status` refreshes cached stat information before comparing file
        # contents. This avoids false dirty results after a build merely
        # updates mtimes, while still detecting staged and unstaged changes.
        status = subprocess.check_output([
            "git",
            str("--git-dir=%s.git" % git_base_path),
            str("--work-tree=%s" % git_base_path),
            "status", "--porcelain", "--untracked-files=no"
        ]).strip()
        if not status:
            return val if val else unknown_label
        return val + b"-dirty" if val else b"unknown-dirty"

    except (subprocess.CalledProcessError, OSError) as e:
        print("git command failed: %s" % str(e))
        return unknown_label


def write_version_info(filename, git_version):
    try:
        # Sanitize git version to prevent invalid C++ code generation
        if b"\"" in git_version or b"\\" in git_version or b"\n" in git_version or b"\r" in git_version:
            print("Warning: git version contains invalid characters, using fallback")
            git_version = b"git_version_is_invalid"  # do not cause build to fail!

        # Try to decode, fallback to safe version if decode fails
        try:
            version_str = git_version.decode("utf-8")
        except UnicodeDecodeError:
            print("Warning: git version contains invalid UTF-8, using fallback")
            version_str = "git_version_decode_error"

        contents = """
/*  Generated by gen_local_config_git.py  */

#ifndef BAZEL_AUTO_GENERATED_VERSION_INFO_H_
#define BAZEL_AUTO_GENERATED_VERSION_INFO_H_

#include <string>
const std::string GIT_VERSION = "%s";

#endif  // BAZEL_AUTO_GENERATED_VERSION_INFO_H_
""" % version_str

        with open(filename, "w") as f:
            f.write(contents)
        print("Generated version info: %s" % version_str)

    except Exception as e:
        print("Error writing version info file %s: %s" % (filename, str(e)))
        # Write a safe fallback version
        fallback_contents = """
/*  Generated by gen_local_config_git.py  */

#ifndef BAZEL_AUTO_GENERATED_VERSION_INFO_H_
#define BAZEL_AUTO_GENERATED_VERSION_INFO_H_

#include <string>
const std::string GIT_VERSION = "unknown_error";

#endif  // BAZEL_AUTO_GENERATED_VERSION_INFO_H_
"""
        with open(filename, "w") as f:
            f.write(fallback_contents)


def generate(arglist):
    spec, head_symlink, _, dest_file = arglist
    data = json.load(open(spec))
    git_version = None
    if not data["git"]:
        git_version = b"unknown"
    else:
        old_branch = data["branch"]
        try:
            new_branch = parse_branch_ref(head_symlink)
            # print("old_branch: " + old_branch + ", new_branch: " + new_branch)
            if new_branch != old_branch:
                print("Warning: branch changed from '%s' to '%s', continuing anyway" %
                      (old_branch, new_branch))
        except Exception as e:
            print(
                "Warning: failed to parse branch reference: %s, continuing anyway" % str(e))

        git_version = get_git_version(data["path"])
    write_version_info(dest_file, git_version)


parser = argparse.ArgumentParser(description="""Git hash injection into bazel.
If used with --configure <path> will search for git directory and put symlinks
into source so that a bazel genrule can call --generate""")

parser.add_argument("--debug",
                    type=bool,
                    help="print debugging information about paths",
                    default=False)

parser.add_argument(
    "--configure",
    type=str,
    help="Path to configure as a git repo dependency tracking sentinel")

parser.add_argument(
    "--gen_root_path",
    type=str,
    help="Root path to place generated git files (created by --configure).")

parser.add_argument(
    "--generate",
    type=str,
    help="Generate given spec-file, HEAD-symlink-file, ref-symlink-file",
    nargs="+")

args = parser.parse_args()

if args.configure is not None:
    if args.gen_root_path is None:
        raise RuntimeError(
            "Must pass --gen_root_path arg when running --configure")
    configure(args.configure, args.gen_root_path, debug=args.debug)
elif args.generate is not None:
    generate(args.generate)
else:
    raise RuntimeError("--configure or --generate must be used")
