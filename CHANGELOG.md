# Changelog

All notable changes to this project are documented here.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and
versioning follows [Semantic Versioning](https://semver.org/) (see
`CLAUDE.md` → "Versioning"). Version lives in `mini-moul/config.sh`
(`VERSION`), which is what `test.sh` prints in its banner.

## [Unreleased]

## [0.3.0] - 2026-07-20

### Added
- Full `ft_printf` test suite: `setup` (Makefile/`ar`/`libftprintf.a` check,
  prototype check), `libc` (all nine mandatory conversions `cspdiuxX%`, plus
  a combined-conversions test), and `bonus` (`-`, `0`, `.` precision,
  minimum field width, `#`, `+`, space flag, and stacked combinations of
  all of the above). Every case compares `ft_printf`'s captured stdout
  output and return value directly against the real `printf` given the
  same format/args, instead of hand-computed expected strings.
- `utils/printf_compare.h`: captures a function's stdout output (via fd
  redirection, since `ft_printf` writes directly rather than through
  buffered stdio) and its return value for byte-exact comparison against
  real `printf`. Reference calls route through a `real_printf` function
  pointer rather than calling `printf()` directly, since some of the most
  useful cases here (conflicting flags, zero-length formats) are exactly
  the ones GCC's static format-string checker (`-Wformat`, part of `-Wall`)
  rejects on a direct call even though the real `printf` handles them fine
  at runtime.

### Changed
- `build_student_objects()` now compiles every `../*.c` at the project
  root, not just `../ft_*.c` — needed since `ft_printf`'s internal helper
  files aren't bound to any naming convention. Any file whose compiled
  object defines `main` is excluded, so a student's own scratch test
  harness can't collide with a test binary's `main()` at link time.
- Added an optional per-assignment `target` file (`tests/<assignment>/target`)
  so every test in every part of an assignment can check compile
  errors/missing-source against one shared source name instead of each
  test's own filename — needed for `ft_printf`, where many test files (one
  per conversion or flag) all exercise the same `ft_printf.c`. Assignments
  without a `target` file (e.g. `libft`) are unaffected.
- `check_prototypes` (in `utils/proto_check.sh`) now takes the student's
  header filename as its first argument instead of hardcoding `libft.h`,
  so it can be reused for `ft_printf.h`. `libft`'s three `prototypes.sh`
  call sites updated to pass `"libft.h"` explicitly; behavior unchanged.

## [0.2.0] - 2026-07-18

### Added
- `test.sh` now scores assignments the way 42's real moulinette does: the
  mandatory parts are worth `/100`, and a `bonus` part is only graded (a flat
  `+25`, capping the final score at `/125`) once the mandatory part is a
  perfect 100. If the mandatory part isn't perfect, `bonus` still runs for
  feedback but doesn't affect the score, and the footer says so. Previously
  every part (including `bonus`) counted as an equal-weight fraction of 100.

## [0.1.1] - 2026-07-18

### Fixed
- `mini-moul.sh` now runs `norminette` like a test case, printing a single
  `PASS`/`FAIL` line instead of dumping all norm errors unconditionally.
  `FAIL` (and the full norminette output) only shows when norminette reports
  at least one error.

## [0.1.0] - 2026-07-12

### Added
- `mini-moul.sh` now checks the `~/mini-moulinette` clone against `origin`
  before running any checks. If it's behind, it asks the user whether to
  fast-forward before continuing; diverged/ahead clones, offline runs, and
  non-interactive shells are left untouched.
- `CHANGELOG.md` (this file) and a "Versioning" section in `CLAUDE.md`
  requiring every PR merged to `main` to bump `VERSION`.

### Changed
- `VERSION` moved out of a hardcoded string in `test.sh`'s banner into
  `mini-moul/config.sh`, as the single source of truth.

### Removed
- The static "Mini moulinette is updated daily. Please remember to git pull
  today!" line from `test.sh`'s footer, superseded by the real update check
  above.

## [0.0.1] - 2026-07-08

### Added
- Initial mini-moulinette fork: `mini-moul.sh` entrypoint, `test.sh` runner,
  and the in-progress `libft` Common Core test suite.
