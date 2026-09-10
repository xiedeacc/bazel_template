# Agent Rules

- Project operating rules must support both Codex and Claude Code. When changing these rules, update both `AGENTS.md` and `CLAUDE.md` in the same change.
- Every library under `src/` is a C++20 named module; read `docs/cpp20_modules.md` before touching module code. Headers never `import`; in every translation unit all `#include` come before all `import`; module interfaces only forward-declare protobuf, grpc stubs, proxygen, folly and boost types, and their bodies live in implementation units.
- A change to module code is verified on every toolchain the project builds with (MSVC, Apple clang, gcc host, aarch64 musl cross) and must pass `bazel build --config=lint` before it is committed.
