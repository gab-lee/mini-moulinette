# Changelog

All notable changes to this project are documented here.

Format follows [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and
versioning follows [Semantic Versioning](https://semver.org/) (see
`CLAUDE.md` → "Versioning"). Version lives in `mini-moul/config.sh`
(`VERSION`), which is what `test.sh` prints in its banner.

## [Unreleased]

## [2.4.0] - 2026-10-03

### Added
- First-cut `get_next_line` suite, checked against subject v14.3.
  `setup`: the three required files, header include guard, the
  `README.md` the subject requires (italic first line, Description,
  Instructions and Resources sections), prototype, builds with every tested
  `BUFFER_SIZE` (1, 2, 5, 42, 9999, 10000000) and without
  `-D BUFFER_SIZE`, no global variables (a file-scope `static` counts),
  no functions beyond `read`/`malloc`/`free`. `mandatory`: 13
  regular-file cases (empty file, missing final `'\n'`, only newlines,
  10000-character lines, 100 lines, every line length from 1 to 60),
  standard input and pipes (including a pipe that stays open, which fails
  an implementation that reads ahead past the first line), and invalid
  fds and `read()` errors. `bonus`: the same file checks on the `_bonus`
  files, at most one static variable, and five interleaved multiple-fd
  cases. Every case runs in a forked child with a 5-second limit, built
  with AddressSanitizer like every other mini test; a case that hits a
  memory error names its kind (e.g. `heap-use-after-free`). Leaks, unfreeable
  lines and reading the whole input on the first call fail; they are
  caught by recompiling the student's files with `malloc`/`free`/`read`
  rerouted to counters in the test driver. Smaller over-reads, and calls
  to `memset`/`memcpy`/`memmove`/`bzero` (which the compiler can
  generate), only print a `[!]` warning, as does a passing case that
  takes more than 1 second (where gnlTester would report TIMEOUT). A case
  that times out skips the remaining cases and buffer sizes of that test.
- Test cases from [Tripouille/gnlTester](https://github.com/Tripouille/gnlTester)
  ported into the get_next_line suite and run with every `BUFFER_SIZE`:
  `mandatory/gnltester.sh` (invalid fds; `files/empty`, `nl`, `41`/`42`/
  `43_no_nl` and `_with_nl` around a 42-byte buffer, `multiple_nlx5`,
  `multiple_line_*`, `alternate_line_nl_*`, the 10000-character
  `big_line_*`; stdin; and, at `BUFFER_SIZE=42`, a `read()` on the fd
  after the first line of `42_with_nl` must still return `'1'`) and
  `bonus/gnltester_bonus.sh` (its multiple-fd sequence with never-opened
  fds 1000 to 1007 in between). Checked against gnlTester itself: both
  pass a correct implementation and fail the same broken ones.
- Runner timeout: every test file (`.c` and `.sh`) runs in its own
  process group and is killed after `TEST_TIMEOUT` seconds (60, in
  `config.sh`) or on Ctrl-C; the FAIL line says `(timed out after 60s)`
  or names the crash signal, e.g. `(crashed: SIGSEGV)`. Sanitizer reports
  from `.sh` tests are now a Memory fail too.
- `README.md`: get_next_line row marked First cut, a note on how the
  suite works, and its three `[!]` warnings listed under Known strictness
  exceptions. The credits now name both of Tripouille's testers
  (libftTester for libft, gnlTester for get_next_line).

### Changed
- `test.sh` adds `exitcode=86` to `ASAN_OPTIONS`, so the get_next_line
  harness can tell a sanitizer report in a forked case from an ordinary
  failure. libft is unaffected: memory errors are still detected from the
  report on stderr.
- The `AddressSanitizer:DEADLYSIGNAL` line printed before a crash report
  no longer leaks into a failing test's output.
- The `bonus` part now always runs last, after any custom-named
  mandatory part.
- `[!]` warnings from a passing `.sh` test are now shown, as they already
  were for `.c` tests.
- `check_prototypes` (in `utils/proto_check.sh`) now takes the student's
  header filename as its first argument instead of hardcoding `libft.h`.
  The libft call sites pass `"libft.h"`; behavior unchanged.

## [2.3.0] - 2026-10-02

### Changed
- A test that hits an AddressSanitizer or LeakSanitizer error now shows a
  red `FAIL <function> Memory fail (<kind>)` with its case lines and one
  `Memory fail:` line (leak size or error kind, plus the student source
  lines involved) instead of the raw sanitizer report.
- Test binaries link `utils/unbuffered_stdout.c`, so case lines printed
  before a sanitizer abort are no longer lost.

## [2.2.0] - 2026-09-30

### Added
- Test cases from [Tripouille/libftTester](https://github.com/Tripouille/libftTester)
  integrated into the libft suite:
  - `utils/alloc_check.h` (`check_alloc_size`, ported from its `mcheck`):
    `ft_strdup`, `ft_calloc`, `ft_substr`, `ft_strjoin`, `ft_strtrim`,
    `ft_itoa`, `ft_strmapi`, `ft_lstnew` and `ft_split` (array and every
    word) must allocate exactly the size needed.
  - `ft_calloc`: `(0, 0)`, `(0, -5)`, `(-5, 0)` must return a pointer;
    `(INT_MAX, INT_MAX)`, `(INT_MIN, INT_MIN)`, `(-5, -5)`, `(3, -5)`,
    `(-5, 3)` must return NULL.
  - `ft_memcpy(dst, NULL, 0)` must return `dst`.
  - `ft_memchr` / `ft_strchr` / `ft_strrchr` with `c + 256` (cast check),
    plus `ft_strrchr("", 'V')` and a last-character match.
  - `ft_strlcpy`: sizes 2, 6, 7, 8 and `-1`, and no write past the NUL.
  - `ft_strlcat`: 16 cases including size `-1`, sizes below the dst
    length, and an empty dst with sizes 0 to 4.
  - `ft_strncmp`: 18 cases including `n = -1` and negative bytes.
  - `ft_strnstr`: 16 cases including `len = -1`, empty haystack/needle,
    and matches that end exactly at or past `len`.
  - `ft_atoi`: whitespace after the sign returns 0, whitespace after a
    digit stops parsing, `--1` / `++1` return 0.
  - `ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_strmapi`,
    `ft_lstadd_back` (appending a whole second list): Tripouille's cases.
- README credits Tripouille/libftTester as a source of the libft cases.
- Run individual functions: `mini strlen split` (the `ft_` prefix is
  optional) runs only those tests, skipping the setup checks, and reports
  `passed/total` instead of a score. `mini -libft strlen` picks the suite
  explicitly, for a folder not named after it. `mini -h` prints usage.
- `mini.sh`: sourced from `~/.zshrc` or `~/.bashrc`, it defines the `mini`
  command with tab completion of suites (`-libft`) and function names, in
  both bash and zsh. The README setup now uses it instead of an alias.

### Changed
- AddressSanitizer is always on (previously opt-in with `MINI_ASAN=1`,
  which is removed). On Linux LeakSanitizer now fails any test that leaks.
  There is no opt-out: if the compiler cannot build with
  `-fsanitize=address`, the run stops with an error. `test.sh` appends
  `allocator_may_return_null=1` (and `detect_leaks=1` on Linux) after any
  user `ASAN_OPTIONS`, so oversized requests return NULL like the real
  allocator and leak checks can't be switched off.
- `ft_calloc(SIZE_MAX, SIZE_MAX)` returning a pointer is now a failure,
  not a `[!]` warning; removed from the README strictness exceptions.

### Fixed
- `ft_calloc` test now enforces the subject's (v19.3) zero-size rule: if
  `nmemb` or `size` is 0, calloc must return a unique pointer that can be
  passed to `free()`. `ft_calloc(0, 8)` previously only had to not crash,
  so a NULL return passed; it now fails, and `ft_calloc(8, 0)` is checked
  too.
- `setup/Makefile.sh` now checks the full rule set the subject requires
  (`$(NAME)`, `all`, `clean`, `fclean`, `re`; `libft.a` and `all` were
  not checked before) and fails a Makefile that relinks when nothing
  changed, which the subject forbids.
- `test.sh` now compiles and links only the student sources for functions
  the subject lists (those with a test file in the suite, plus `_bonus`
  variants). It previously built every `../ft_*.c`, so an extra accessory
  file with a `main()` or a duplicate symbol broke every test's link.

### Removed
- `mini-moul/tests/42Piscine(archive)/` (piscine C00 to C08 tests) and the
  `(archive)` skip logic in `mini-moul.sh` and `test.sh`. The runner never
  executed them; piscine tests live in the original k11q/mini-moulinette.

## [2.1.0] - 2026-09-30

### Added
- `MINI_ASAN=1` opt-in env var (e.g. `MINI_ASAN=1 ./test.sh libft`) compiles
  and links every student object and test binary with
  `-fsanitize=address`, in addition to the existing `-Wall -Werror
  -Wextra`. Off by default so it never changes existing scoring. Pure
  output/return-value comparison can pass code that still corrupts memory
  (a 1-byte-short `malloc`, a use-after-free) - this catches that class of
  bug directly, as long as a test happens to touch the bad byte. Note
  LeakSanitizer isn't supported on macOS/arm64, so this is
  AddressSanitizer only; a plain unfreed-but-otherwise-safe allocation
  still won't be reported.

### Fixed
- `ft_putnbr_fd` test only checked `0, 42, -42, INT_MAX, INT_MIN` - none
  of which ever make a recursive digit-printer's `n /= 10` chain pass
  through exactly `10`, the single value that exposes the common
  off-by-one `if (n > 10)` (instead of `if (n >= 10)`) recursion guard.
  Added cases for `10`, `100`, `1000`, `105`, `-10`, and `10000000`.
- `ft_lstmap` test only exercised the all-succeed path. Added a case
  where the mapping function fails (returns `NULL`) on the second of
  three nodes, asserting the call returns `NULL` and that the *original*
  list survives untouched - the highest-risk branch in this function
  (freeing the partially-built new list via `del` without disturbing the
  input).

## [2.0.2] - 2026-09-28

### Fixed
- `ft_split` test only exercised short inputs (at most two multi-character
  words) and only `' '` and `','` as separators. Added cases for:
  - multiple words: `"hello world foo"`, four-, nine- and twenty-word
    strings, and long words of differing lengths;
  - whitespace separators: `'\t'`, `'\n'`, `'\v'`, `'\f'`, `'\r'`, and a
    tabs-only string;
  - whitespace that is not the separator: tabs and other whitespace must
    stay inside words when splitting on `' '`, and spaces must stay inside
    words when splitting on `'\t'`;
  - a `'\0'` separator returning the whole string as one element.
- `ft_split` failure messages now print whitespace escaped (`\t`, `\n`, ...)
  instead of raw, and an unexpectedly non-empty result for an empty-array
  case is reported as such instead of "past index -1".

## [2.0.1] - 2026-09-23

### Fixed
- `ft_lstsize`'s checked prototype was `int ft_lstsize(t_list *lst)` in both
  `tests/libft/linked_list/prototypes.sh` and `tests/libft/libft_proto.h`,
  but the subject specifies `unsigned int`. This rewarded a wrong `int`
  signature and would hard-fail a subject-correct one. Fixed both, and
  updated `ft_lstsize.c`'s local variables to `unsigned int` to match.

## [2.0.0] - 2026-09-23

### Changed
- **Breaking:** `libft`'s linked-list part (`ft_lstnew`, `ft_lstadd_front`,
  `ft_lstsize`, `ft_lstlast`, `ft_lstadd_back`, `ft_lstdelone`,
  `ft_lstclear`, `ft_lstiter`, `ft_lstmap`) moved from `tests/libft/bonus/`
  to `tests/libft/linked_list/`. The subject now lists it as "Part 3" under
  the mandatory part, not a separate bonus chapter, so it's no longer
  graded as optional `+25` extra credit gated on a perfect mandatory
  score — it now counts as an ordinary mandatory part toward the 100%.
- `libc_compare.h`'s `check_truthy`/`sweep_truthy` (used by `isalpha`,
  `isdigit`, `isalnum`, `isascii`, `isprint`) now require an exact `1` or
  `0` return, matching the subject's explicit requirement, instead of
  accepting any nonzero value as "true".

## [1.0.1] - 2026-09-23

### Removed
- Norminette check. It ran as an ordinary `libft` `setup`-part test case
  (`tests/libft/setup/norminette.sh`, `utils/norminette_check.sh`) but caused
  bugs, so it's gone. Norm compliance is no longer graded by
  mini-moulinette — run `norminette` yourself before submitting.

### Changed
- `README.md`: added a "Get Started" note reminding students to run
  `norminette` themselves, since mini-moulinette no longer checks it.

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
