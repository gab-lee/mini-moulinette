# Changelog

All notable changes to this project are documented here.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and
versioning follows [Semantic Versioning](https://semver.org/) (see
`CLAUDE.md` → "Versioning"). Version lives in `mini-moul/config.sh`
(`VERSION`), which is what `test.sh` prints in its banner.

## [Unreleased]

## [1.0.0] - 2026-07-18

### Changed
- The project is now **live**. Removed the "not live yet" warning banner and
  the matching caution note in "Get Started" from `README.md`.
- `README.md` overhaul: moved "Get Started" up to directly follow "How Does
  It Work?"; reworked "Updating" to describe the automatic update check
  `mini-moul.sh` already performs on every run (manual `git pull` is now
  presented as a fallback); replaced the stale piscine `C05` segfault-
  debugging example with a real, current `libft/libc/ft_isalpha.c` one;
  renamed "Contributing" to "Contribution"; simplified the "License" section
  to drop the now-unnecessary attribution link; fixed links to Khairul
  Haaziq's profile/repo to point at their current handle
  (`https://github.com/k11q`).
- Roadmap: marked Circle 0 (Libft) complete, noted Circle 1 (ft_printf,
  get_next_line) is targeting end of August, and dropped the "Exam practice
  mode" line and the stale "(not just C00–C13)" qualifier. Coverage Status
  table updated to match (Libft: Complete; ft_printf/get_next_line: Planned,
  targeting end of Aug).

## [0.3.0] - 2026-07-18

### Added
- `mini-moul/utils/norminette_check.sh`, a shared `check_norminette` helper
  (mirrors `proto_check.sh`) that any assignment's `setup` part can source to
  add a norminette check as an ordinary test case.
- `tests/libft/setup/norminette.sh`, using the helper above — norminette now
  runs as part of `libft`'s normal `setup` checks instead of a separate step.

### Changed
- `mini-moul.sh` no longer runs norminette as a standalone pre-step with its
  own PASS/FAIL line before the test banner; that was distracting and
  disconnected from the rest of the run. norminette now only shows up as a
  regular `setup` test, folded into the normal PASS/FAIL list and scoring.

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
