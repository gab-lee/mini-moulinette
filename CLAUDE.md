# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

Mini-moulinette is a bash-based test runner for 42 School assignments. This fork extends the original piscine-focused tool to the New Common Core curriculum (Circles 0–6). It is **live** — Circle 0 (`libft`) is complete and `get_next_line` has a first-cut suite; the rest of the curriculum is being added circle by circle (see README "Roadmap").

## Running tests

There is no build step; everything runs via bash + `cc`.

- Run the full suite for an assignment (from inside a student project directory, e.g. `libft/`):
  ```bash
  ~/mini-moulinette/mini-moul.sh
  ```
  This copies `mini-moul/` into the current directory, detects the assignment from the directory's basename (must match a folder in `mini-moul/tests/`), executes `test.sh <assignment>`, then cleans up. norminette is **not** run — see the note below.

- To run directly without the copy/cleanup wrapper (useful while iterating on tests), from inside `mini-moul/`:
  ```bash
  ./test.sh <assignment>          # e.g. ./test.sh libft
  ```

- AddressSanitizer is **always on**, with no opt-out: `CC_FLAGS` in `test.sh` includes `-fsanitize=address -g` on top of `-Wall -Werror -Wextra` for every compile/link step (student objects and test binaries). `test.sh` probes `cc -fsanitize=address` at startup and exits with an error if it fails, rather than grading without it. It appends `allocator_may_return_null=1` and `exitcode=86` (plus `detect_leaks=1` on Linux, so LeakSanitizer fails leaking tests; unsupported on macOS/arm64) after any user `ASAN_OPTIONS`, so those can't be switched off. Test files must free everything they allocate, or they fail under LeakSanitizer. Extend `CC_FLAGS`, not the individual `cc` invocations, if you add another flag. A sanitizer report on a test's stderr is turned into a red `Memory fail` (`memory_error_kind`/`memory_error_detail` in `test.sh`); `utils/unbuffered_stdout.c` is linked into every test binary so case lines printed before a sanitizer abort are not lost.
  Note `test.sh` expects the student project one level up (`../ft_*.c`, `../libft.h`, etc.), so run it from a copy of `mini-moul/` placed inside the student project — mirror what `mini-moul.sh` does, or symlink a test project's sources into a scratch `../` for a quicker loop.

- Run a single test binary directly once compiled, e.g.:
  ```bash
  cc -Wall -Werror -Wextra -o t tests/libft/libc/ft_strlen.c <student objects> && ./t
  ```

## Architecture

**`mini.sh`** (repo root) — sourced from the user's `~/.zshrc`/`~/.bashrc`. Defines `mini` as a shell function (removing any old `alias mini`) and registers `_mini_complete` via `complete` (zsh through `bashcompinit`): it completes `-<suite>` as the first word and the suite's function names from `mini-moul/tests/<suite>/*/*.c` (bare names like `strlen`, or `ft_`-prefixed once the word starts with `f`).

**`mini-moul.sh`** (repo root) — user-facing entrypoint: `mini [-<suite>] [function ...]`. Takes the suite from `-<suite>` or else the current directory's basename, validates it matches a known test suite, stages a throwaway copy of `mini-moul/`, delegates to `test.sh <suite> [function ...]`, and removes the copy afterward (also on `SIGINT`).

**`mini-moul/test.sh`** — the actual runner, invoked as `./test.sh <assignment> [function ...]`. Function names (with or without `ft_`) put it in selected mode: `select_functions()` validates them against the suite's test files, only those `.c` tests run (setup `.sh` scripts and parts without a selected function are skipped), and `print_selected_footer()` reports `passed/total` instead of a score. Otherwise:
- Finds `tests/<assignment>/`, iterates its subdirectories ("parts": `setup`, `libc`, `additional` in that order, then any others, then `bonus` last) as scoring units. A part literally named `bonus` is graded separately (flat `+25`, only once the mandatory parts hit 100%) — `libft`'s linked-list part is mandatory per the subject, so it's named `linked_list`, not `bonus`, and counts as an ordinary mandatory part.
- `build_student_objects()` compiles only the student sources named after a test file in the suite (`../<fn>.c` or `../<fn>_bonus.c`, i.e. the subject's function list; extra accessory `ft_*.c` files are ignored) once into `.o` files (capturing compile errors per-file into `.student_objs/*.err`); these objects are linked into each test binary rather than recompiling the student's sources repeatedly.
- Within a part, `collect_tests()` lists test files, honoring an optional `order` file (one filename per line) so tests run in subject order; anything not listed is appended after.
- Each test file is either:
  - a `.c` file: compiled with `cc -Wall -Werror -Wextra`, linked against the precompiled student objects, and run — exit 0 is PASS, and stdout/stderr with a `[!]` marker on an otherwise-passing test is a known-strictness warning (see README);
  - a `.sh` script (used for `setup` parts like prototype/header checks): run directly with the mini-moul directory as `cwd` and the student project at `../` — exit 0 is PASS.
- Every test (`.c` or `.sh`) runs through `run_test()` → `run_limited()`: a `perl` wrapper (macOS has no `timeout(1)`) that runs it in its own process group, kills the whole group after `TEST_TIMEOUT` seconds (`config.sh`) or on Ctrl-C, and reports `(timed out after Ns)` or `(crashed: SIGSEGV)` on the FAIL line. Tests run with stdin from `/dev/null`; a sanitizer report on stderr is a Memory fail for `.sh` tests too, and `[!]` lines of a passing `.sh` test are shown like those of a `.c` test.
- Scoring mirrors 42's own moulinette: if any check in a part fails, that part's score doesn't count (`break_score`), and score is `passed_parts / total_parts * 100`.

**`mini-moul/tests/<assignment>/<part>/`** — one directory per assignment (e.g. `libft`, `ft_printf`, `philo`), each with subdirectories per part. Test files are named after the function/program they test (e.g. `libft/libc/ft_strlen.c` tests `ft_strlen`). Each `.c` test:
- `#include`s a local prototypes header (e.g. `libft_proto.h`) declaring the functions under test — the student's own source/headers are **never** `#include`d, only their compiled `.o` linked in, so student helpers/macros/static functions can't leak into or break the test.
- Uses shared helpers from `mini-moul/utils/` (e.g. `libc_compare.h` for `check_truthy`/`check_exact`/sweep helpers that compare `ft_*` output against the real libc function instead of hardcoded expected values; `constants.h` for shared color/formatting macros; `alloc_check.h` for `check_alloc_size`, which compares a returned block's usable size against `malloc(required)`, ported from Tripouille/libftTester's `mcheck`).
- Declares cases as a `t_test` struct array (see README "Debugging" section for the shape) or as straight-line assertions with `check_*` calls, one numbered case per behavior, each printing a PASS/FAIL line.

**`mini-moul/tests/get_next_line/`** — get_next_line has no Makefile and must work with any `BUFFER_SIZE`, so all its tests are `.sh` scripts built on `gnl_harness.sh`, written against subject v14.3. `setup/readme.sh` checks the `README.md` the subject's chapter V requires (italic first line, Description/Instructions/Resources headings; the algorithm explanation can't be checked). `setup/compile.sh` (and `bonus/compile_bonus.sh`) build the student's two files once per size in `GNL_SIZES` (`default` = no `-D BUFFER_SIZE`, then 1 … 10000000) into `.gnl_build/`, each linked with `gnl_driver.c`; the other scripts run the driver's scenarios (`file`, `stdin`, `errors`, `multi`, and `tripouille`/`tripouille_multi`, the cases ported from Tripouille/gnlTester with its exact file contents in `g_tri_files`) against every build. The student's files are compiled a second time with `-Dmalloc=gnl_test_malloc -Dfree=gnl_test_free -Dread=gnl_test_read` so the driver can count leaked blocks, check returned lines are freeable, see how far `get_next_line` read ahead, and make `read()` fail; if only that instrumented build fails, the plain build is used and those checks are skipped with a `[!]` note. Every case runs in a forked child with a 5-second alarm, so a crash, infinite loop or blocking `read()` is reported against its case number and buffer size. The instrumented objects, the driver and the links use `-fsanitize=address -g` (`GNL_ASAN`), like every other test; the plain objects do not, since sanitizer symbols would confuse the `nm` checks. A sanitizer report exits with 86 (`ASAN_OPTIONS` from `test.sh`), which the driver reads as "memory error in this case" (each case's stderr is captured to name the error kind, then passed through), while `test.sh` turns the report on stderr into the Memory fail line. `setup/forbidden.sh` uses `nm` on the objects to reject global variables (a file-scope `static` counts; a function's static is told apart by the dot the compiler puts in its symbol name) and calls outside `read`/`malloc`/`free` (and, in the bonus, more than one static variable); `memset`/`memcpy`/`memmove`/`bzero` only get a `[!]` warning, since the compiler can emit them. Reading the whole input on the first call fails; any smaller over-read is a `[!]` warning.

**`mini-moul/tests/<assignment>/*/prototypes.sh`** — setup-part scripts that call `check_prototypes` (defined in `mini-moul/utils/proto_check.sh`) with the student's header filename (e.g. `"libft.h"`, `"get_next_line.h"`) followed by `"name|pointer-decl|subject prototype"` triples. Each entry compiles a tiny probe assigning the student's function to a function-pointer of the exact subject-declared type; because C doesn't check types at link time, this is the only place a wrong signature (return type, param types, missing `const`, ...) gets caught, distinct from behavioral tests.

**No norminette check.** Norm compliance is not part of any assignment's `setup` part — it caused bugs (false PASS/FAIL against the student's actual norm state) and was removed. Students must run `norminette` themselves before submitting; 42's real moulinette still enforces it.

**`mini-moul/utils/`** — shared C headers/shell helpers included by tests across assignments (`constants.h`, `libc_compare.h`, `alloc_check.h`, `proto_check.sh`). Prefer extending these over duplicating comparison logic in individual test files.

**Adding a new assignment/part**: create `mini-moul/tests/<assignment>/<part>/`, add `.c`/`.sh` test files (optionally an `order` file), and if needed a `prototypes.sh` sourcing `proto_check.sh`. `test.sh` picks it up automatically — no registration needed elsewhere.

**Known strictness exceptions**: some libc-comparison tests are intentionally stricter than the real 42 moulinette (e.g. `ft_strchr`/`ft_strrchr` with extended chars); these print a `[!]` warning rather than failing. See README "Coverage Status" for the current list — don't silently loosen or tighten these without updating both the test and the README.

## Versioning

`mini-moul/config.sh` defines `VERSION` (SemVer `MAJOR.MINOR.PATCH`) — the single source of truth, printed in `test.sh`'s banner. **Every PR merged to `main` must bump it and add an entry to `CHANGELOG.md`**, in the same commit/PR as the change:
- `PATCH` — bug fixes, test corrections, wording/output tweaks.
- `MINOR` — new assignment/part test coverage, new features (e.g. the update-notification check), backwards-compatible additions.
- `MAJOR` — breaking changes to the runner/test framework (e.g. a scoring-model or test-file-format change that invalidates existing assignments), or a curriculum-completion milestone (the project reached `1.0.0` when it went live with Circle 0 complete).

`CHANGELOG.md` follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/): move the relevant bullets out of `[Unreleased]` into a new dated `[MAJOR.MINOR.PATCH] - YYYY-MM-DD` section (categorized under Added/Changed/Fixed/Removed) as part of the same PR that bumps `VERSION`.
