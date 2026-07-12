# Changelog

All notable changes to this project are documented here.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and
versioning follows [Semantic Versioning](https://semver.org/) (see
`CLAUDE.md` → "Versioning"). Version lives in `mini-moul/config.sh`
(`VERSION`), which is what `test.sh` prints in its banner.

## [Unreleased]

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
