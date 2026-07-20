# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Mini-moulinette is a bash-based test runner for 42 School assignments. This fork extends the original piscine-focused tool to the New Common Core curriculum (Circles 0–6). It is **not live yet** — no Common Core test suites are complete (`libft` is in progress, `ft_printf` is done).

## Running tests

There is no build step; everything runs via bash + `cc`.

- Run the full suite for an assignment (from inside a student project directory, e.g. `libft/`):
  ```bash
  ~/mini-moulinette/mini-moul.sh
  ```
  This copies `mini-moul/` into the current directory, detects the assignment from the directory's basename (must match a folder in `mini-moul/tests/`, and not contain `(archive)`), runs norminette if installed, executes `test.sh <assignment>`, then cleans up.

- To run directly without the copy/cleanup wrapper (useful while iterating on tests), from inside `mini-moul/`:
  ```bash
  ./test.sh <assignment>          # e.g. ./test.sh libft
  ```
  Note `test.sh` expects the student project one level up (`../ft_*.c`, `../libft.h`, etc.), so run it from a copy of `mini-moul/` placed inside the student project — mirror what `mini-moul.sh` does, or symlink a test project's sources into a scratch `../` for a quicker loop.

- Run a single test binary directly once compiled, e.g.:
  ```bash
  cc -Wall -Werror -Wextra -o t tests/libft/libc/ft_strlen.c <student objects> && ./t
  ```

## Architecture

**`mini-moul.sh`** (repo root) — user-facing entrypoint. Validates the current directory matches a known test suite, stages a throwaway copy of `mini-moul/`, delegates to `test.sh`, and removes the copy afterward (also on `SIGINT`).

**`mini-moul/test.sh`** — the actual runner, invoked as `./test.sh <assignment>`:
- Finds `tests/<assignment>/`, iterates its subdirectories ("parts": conventionally `setup`, `libc`, `additional`, `bonus`, run in that order, then any others) as scoring units.
- `build_student_objects()` compiles every `../*.c` at the project root once into `.o` files (capturing compile errors per-file into `.student_objs/*.err`); these objects are linked into each test binary rather than recompiling the student's sources repeatedly. Any file whose compiled object defines `main` is excluded, so a student's own scratch test harness can't collide with a test binary's `main()` at link time.
- Within a part, `collect_tests()` lists test files, honoring an optional `order` file (one filename per line) so tests run in subject order; anything not listed is appended after.
- Each test file is either:
  - a `.c` file: compiled with `cc -Wall -Werror -Wextra`, linked against the precompiled student objects, and run — exit 0 is PASS, and stdout/stderr with a `[!]` marker on an otherwise-passing test is a known-strictness warning (see README);
  - a `.sh` script (used for `setup` parts like prototype/header checks): run directly with the mini-moul directory as `cwd` and the student project at `../` — exit 0 is PASS.
- Scoring mirrors 42's own moulinette: if any check in a part fails, that part's score doesn't count (`break_score`), and score is `passed_parts / total_parts * 100`.
- Compile-error/missing-source messages check against a `target` name (`test_target_name()`), which defaults to the test file's own basename (libft: one test file per function, e.g. `ft_strlen.c` checks for `../ft_strlen.c`) but can be overridden per assignment by dropping a `tests/<assignment>/target` file containing a single name — every test in every part of that assignment then checks against that one shared source instead (ft_printf: every test, regardless of which conversion/flag it exercises, checks against `../ft_printf.c`, since it's all one function).

**`mini-moul/tests/<assignment>/<part>/`** — one directory per assignment (e.g. `libft`, `ft_printf`, `philo`), each with subdirectories per part. Test files are named after the function/program they test (e.g. `libft/libc/ft_strlen.c` tests `ft_strlen`; for a `target`-based assignment like ft_printf, the filename instead names the specific behavior under test, e.g. `ft_printf_flag_hash.c`). Each `.c` test:
- `#include`s a local prototypes header (e.g. `libft_proto.h`, `ft_printf_proto.h`) declaring the functions under test — the student's own source/headers are **never** `#include`d, only their compiled `.o` linked in, so student helpers/macros/static functions can't leak into or break the test.
- Uses shared helpers from `mini-moul/utils/` (e.g. `libc_compare.h` for `check_truthy`/`check_exact`/sweep helpers that compare `ft_*` output against the real libc function instead of hardcoded expected values; `printf_compare.h` for `check_printf`, which captures `ft_printf`'s stdout output + return value via fd redirection and compares byte-exact against the real `printf` given the same format/args; `constants.h` for shared color/formatting macros).
- Declares cases as a `t_test` struct array (see README "Debugging" section for the shape) or as straight-line assertions with `check_*` calls, one numbered case per behavior, each printing a PASS/FAIL line.

**`mini-moul/tests/<assignment>/*/prototypes.sh`** — setup-part scripts that call `check_prototypes` (defined in `mini-moul/utils/proto_check.sh`) with the student's header filename (e.g. `"libft.h"`, `"ft_printf.h"`) followed by `"name|pointer-decl|subject prototype"` triples. Each entry compiles a tiny probe assigning the student's function to a function-pointer of the exact subject-declared type; because C doesn't check types at link time, this is the only place a wrong signature (return type, param types, missing `const`, ...) gets caught, distinct from behavioral tests.

**`mini-moul/utils/`** — shared C headers/shell helpers included by tests across assignments (`constants.h`, `libc_compare.h`, `printf_compare.h`, `proto_check.sh`). Prefer extending these over duplicating comparison logic in individual test files.

**Adding a new assignment/part**: create `mini-moul/tests/<assignment>/<part>/`, add `.c`/`.sh` test files (optionally an `order` file), and if needed a `prototypes.sh` sourcing `proto_check.sh` with the student's header filename. Drop a `tests/<assignment>/target` file if the assignment's tests don't map 1:1 to student source filenames (see ft_printf). `test.sh` picks it up automatically — no registration needed elsewhere.

**Known strictness exceptions**: some libc-comparison tests are intentionally stricter than the real 42 moulinette (e.g. `ft_strchr`/`ft_strrchr` with extended chars, `ft_calloc(SIZE_MAX, SIZE_MAX)`); these print a `[!]` warning rather than failing. See README "Coverage Status" for the current list — don't silently loosen or tighten these without updating both the test and the README.

## Versioning

`mini-moul/config.sh` defines `VERSION` (SemVer `MAJOR.MINOR.PATCH`) — the single source of truth, printed in `test.sh`'s banner. **Every PR merged to `main` must bump it and add an entry to `CHANGELOG.md`**, in the same commit/PR as the change:
- `PATCH` — bug fixes, test corrections, wording/output tweaks.
- `MINOR` — new assignment/part test coverage, new features (e.g. the update-notification check), backwards-compatible additions.
- `MAJOR` — reserved; stays `0.x.y` until the Common Core suites are actually live (see "What this is").

`CHANGELOG.md` follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/): move the relevant bullets out of `[Unreleased]` into a new dated `[MAJOR.MINOR.PATCH] - YYYY-MM-DD` section (categorized under Added/Changed/Fixed/Removed) as part of the same PR that bumps `VERSION`.
