# Mini-moulinette — New Common Core

[![Buy Me A Beer](https://img.shields.io/badge/%F0%9F%8D%BA-Buy%20Me%20A%20Beer-FFDD00?style=for-the-badge)](https://buymeacoffee.com/gablee)

![mini-moulinette](mini-moulinette.jpg)

Mini-moulinette is a test runner for 42 assignments. It runs a full suite of automated tests over an assignment with a single command, so you can check your code thoroughly *before* submitting — instead of finding out the hard way at evaluation.

This repository is a fork that extends mini-moulinette to the **New Common Core** curriculum (Circles 0–6).


## Credits

All credit for the original mini-moulinette goes to **[Khairul Haaziq](https://github.com/k11q)** — the original project lives at [k11q/mini-moulinette](https://github.com/k11q/mini-moulinette).

This tool was incredibly useful during my piscine: it saved me countless hours of waiting for evaluations only to fail on silly mistakes. This fork exists because I want the same safety net while going through the Common Core. Thank you, Khairul! 🙏

The libft and get_next_line suites also integrate the test cases from **[Tripouille](https://github.com/Tripouille)**'s testers: **[libftTester](https://github.com/Tripouille/libftTester)** for libft, including its exact allocation-size checks, and **[gnlTester](https://github.com/Tripouille/gnlTester)** for get_next_line, whose files, call sequences and multiple-fd bonus run in mini with every `BUFFER_SIZE`.


## How Does It Work?

![screenshot](screenshot.jpg)

- Mini-moulinette runs through all the test cases for an assignment automatically and checks that the expected conditions are met.
- It then prints a per-exercise pass/fail summary.
- Scoring follows 42's practice: if an earlier exercise fails, the following ones don't count.


## Get Started

> ***Warning***
> Mini-moulinette is not 100% accurate — the tests may not cover every edge case the real moulinette does. Use it as a safety net, not a guarantee.

> [!IMPORTANT]
> Mini-moulinette does **not** check norminette compliance. Run `norminette` yourself before submitting — the real 42 moulinette still enforces it, and a norm error there means a `0`.

1. Clone the repository into `~/mini-moulinette` (the runner expects this path):

```bash
git clone https://github.com/gab-lee/mini-moulinette.git ~/mini-moulinette
```

2. Load the `mini` command (this also enables tab completion).

- zsh:

```zsh
echo "source ~/mini-moulinette/mini.sh" >> ~/.zshrc && source ~/.zshrc
```

- bash:

```bash
echo "source ~/mini-moulinette/mini.sh" >> ~/.bashrc && source ~/.bashrc
```

If you set mini up with the older `alias mini=...` line, delete that line from your rc file, otherwise the alias overrides the command and tab completion does not work.

3. Go to the project directory you want to test, e.g. `libft`:

```bash
cd libft
```

4. Run `mini` — it detects the project from the folder name:

```bash
mini
```

5. That's it — run it in every project directory where tests are provided. Have fun!

### Testing individual functions

Pass function names to run only those tests. The `ft_` prefix is optional, and Tab completes the names:

```bash
mini strlen            # only ft_strlen
mini ft_split substr   # several functions
mini -libft strlen     # pick the suite yourself when your folder has another name
mini -h                # usage, including --show and --try
```

A run limited to some functions skips the setup checks (Makefile, `libft.h`, prototypes) and shows how many of the chosen functions passed instead of a score. Run `mini` with no names for the full graded suite.

### Showing every test case

By default a passing function prints only its PASS line. Add `--show` to also print every case it ran:

```bash
mini --show            # whole suite, every case
mini --show strlen     # every case of ft_strlen
```

### Trying a function by hand (libft)

`--try` calls one of your functions with your own arguments and prints what it returned, next to the real libc result when there is one (`ft_strlcpy`, `ft_strlcat` and `ft_strnstr` are compared with a BSD reference, since not every libc has them):

```bash
mini --try strlen "hello"
mini --try memchr 'ab\0cd' c 5
mini --try substr "hello world" 6 5
mini --try strmapi "hello" toupper
mini --try lstsize a b c
```

```
ft_strlen("hello")
  ft     5
  libc   5
  ✓ same result as libc
```

Each argument is one shell word, so quote anything with spaces:

- **string**: `\n`, `\t`, `\0`, `\xHH` and the other C escapes are decoded (use single quotes so the shell leaves the backslash alone). `@null` passes a `NULL` pointer.
- **number**: base 10, e.g. `42` or `-1`. A negative `size_t` wraps, so `-1` is `SIZE_MAX`.
- **char**: one character is taken as is (`a`, `' '`, `7` is `'7'`); anything longer is its value (`0`, `200`, `-1`). `\0` is the NUL byte.
- **buffers** (`ft_memset`, `ft_memcpy`, `ft_bzero`, ...) are exactly as big as the string you give plus its NUL, so going past the end is reported. `ft_memmove` takes one buffer and two offsets into it, so you can test overlapping copies.
- **lists** (`ft_lst*`): one argument per node. `ft_lstadd_front`/`ft_lstadd_back` take the new node first.
- **callbacks** (`ft_strmapi`, `ft_striteri`, `ft_lstiter`, `ft_lstmap`): `toupper`, `tolower`, `addindex` (adds the index to the character) or `digit` (replaces it with its index's last digit).
- **fd** (`ft_put*_fd`): optional, default `1`. Output to fd 1 is captured and shown escaped; other fds are written to directly.

A wrong number of arguments prints what that function takes. `--try` runs under AddressSanitizer like the tests, so a crash, leak or overflow in the call is reported as a `Memory fail`.


## Memory Safety

Every test is always compiled and linked with
[AddressSanitizer](https://github.com/google/sanitizers/wiki/AddressSanitizer),
so memory bugs that do not change the visible result still
fail: a `malloc` one byte too short, a heap-buffer-overflow, a
use-after-free. On Linux, LeakSanitizer also fails any test that leaks
memory. LeakSanitizer is not supported on macOS/arm64, so there leaks are
not reported.

A memory error fails the function with a red `Memory fail` instead of the
raw sanitizer report: the test's case lines are shown, followed by one
line naming the error and where it happened in your code, e.g.
`Memory fail: 24 byte(s) leaked in 6 allocation(s), at ft_strjoin.c:3`.

get_next_line is built with AddressSanitizer at every `BUFFER_SIZE`, and
each failing case says it hit a memory error.

Tests that return a newly allocated block (`ft_strdup`, `ft_calloc`,
`ft_substr`, `ft_strjoin`, `ft_strtrim`, `ft_split`, `ft_itoa`,
`ft_strmapi`, `ft_lstnew`) also check that it is exactly the size needed,
e.g. `strlen + 1` for a string.

There is no way to turn it off. If your compiler cannot build with
`-fsanitize=address`, mini stops with an error instead of grading without
it.


## Roadmap

The original project covers the piscine (C00–C08). The goal of this fork is to progressively add test suites for the Common Core, circle by circle:

> [!NOTE]
> **A caveat:** I haven't started the 42 Cursus yet — I'll only be starting in September. A lot of the project list and scope below is pieced together from PDFs found online, so it may not match the current curriculum exactly. If you're already in the programme and would like to contribute, access to up-to-date materials would also be greatly appreciated!

- [x] Adapt the runner to detect Common Core project directories
- [x] **Circle 0** — Libft (Part 1, Part 2, Part 3 - linked list)
- [ ] **Circle 1** — ft_printf, get_next_line (mandatory + bonus) — get_next_line first cut done
- [ ] **Circle 2** — push_swap (operation validity + sort check), minitalk / pipex
- [ ] **Circle 3** — philosophers (death timing / no-death scenarios), minishell (command comparison against bash)
- [ ] **Circle 4** — CPP Modules 00–04
- [ ] **Circle 5** — CPP Modules 05–09, webserv / ft_irc basic conformance tests

Projects that are graphical, system-administration or web-based (Born2beroot, so_long / FdF / fract-ol, NetPractice, cub3D / miniRT, inception, ft_transcendence) don't lend themselves well to automated unit testing, so they are out of scope for now — the table below still lists them for completeness.


## Coverage Status

> Looking for **piscine** tests (C00–C08)? This fork focuses on the New Common Core — head over to the original [k11q/mini-moulinette](https://github.com/k11q/mini-moulinette) for the piscine test suites.

| Circle | Project                    | Exercises / Parts to cover                 | Coverage        | Cross-tested against 42 submissions |
| :----: | :------------------------- | :----------------------------------------- | :-------------: | :---------------------------------- |
| 0      | Libft                      | Part 1 (libc), Part 2 (additional), Part 3 (linked list) | Complete | [Mia Combeau](https://github.com/mcombeau/libft/tree/main) |
| 1      | ft_printf                  | Mandatory conversions + bonus flags        | Planned (targeting end of Aug) | —                     |
| 1      | get_next_line              | Every `BUFFER_SIZE` from 1 to 10000000, files, stdin and pipes, invalid fds, leaks, multiple-fd bonus | First cut | —                     |
| 1      | Born2beroot                | —                                          | Out of scope (VM / sysadmin) | —                       |
| 2      | push_swap                  | Operation validity, sort check, op count   | Planned         | —                                    |
| 2      | minitalk / pipex           | Signal transmission / pipe behaviour       | Planned         | —                                    |
| 2      | so_long / FdF / fract-ol   | —                                          | Out of scope (graphical) | —                           |
| 3      | philosophers               | Death timing, no-death scenarios           | Planned         | —                                    |
| 3      | minishell                  | Output comparison against bash             | Planned         | —                                    |
| 4      | NetPractice                | —                                          | Out of scope (web exercise) | —                        |
| 4      | cub3D / miniRT             | —                                          | Out of scope (graphical) | —                           |
| 4      | CPP Modules 00–04          | Per-exercise behaviour tests               | Planned         | —                                    |
| 5      | CPP Modules 05–09          | Per-exercise behaviour tests               | Planned         | —                                    |
| 5      | webserv / ft_irc           | Basic protocol conformance                 | Planned         | —                                    |
| 5      | inception                  | —                                          | Out of scope (Docker infra) | —                        |
| 6      | ft_transcendence           | —                                          | Out of scope (web project) | —                         |

> [!NOTE]
> **Known strictness exceptions.** Some checks are stricter than 42's moulinette, or cannot tell a real mistake from something the compiler did. These cases print a yellow `[!]` warning instead of failing:
> - `ft_strchr` / `ft_strrchr` searching for an extended character (e.g. 233): implementations that compare as `unsigned char` return NULL where libc finds the byte.
> - get_next_line reading further ahead than it needs to (for example one extra `read()` per call): the subject says to read as little as possible, but only reading the whole file on the first call fails.
> - get_next_line calling `memset`, `memcpy`, `memmove` or `bzero`: the compiler can generate these calls on its own (for example to zero an array), so they are not failed, but calling them yourself is forbidden.
> - get_next_line taking more than 1 second on a case: Tripouille's gnlTester would report TIMEOUT, but mini only fails a case after 5 seconds. The warning is shown once per test, for the first slow case.

> [!NOTE]
> **get_next_line** is compiled and run with every `BUFFER_SIZE` in 1, 2, 5, 42, 9999 and 10000000, and once without `-D BUFFER_SIZE` (the subject requires both). Each case runs on its own with a 5-second limit, so an infinite loop, a crash or a `read()` that waits forever fails that one case instead of hanging the run; a failure shows the case, the buffer size, the line expected and the line returned. Setup also checks the `README.md` the subject requires (italic first line, Description, Instructions and Resources sections); the algorithm explanation it asks for is up to you and your peers. The `gnltester` tests port every case from [Tripouille/gnlTester](https://github.com/Tripouille/gnlTester), including its check that, at `BUFFER_SIZE=42`, nothing past the first line of `files/42_with_nl` was read. The tests also check for memory leaks after `get_next_line` returns `NULL`, reject global variables and functions other than `read`, `malloc` and `free`, and fail an implementation that reads the whole file before returning the first line. Run it from a folder named `get_next_line`, or with `mini -get_next_line` from any folder.


## Updating

Mini-moulinette checks for updates automatically — every time you run `mini`, it fetches `origin` and, if your local clone is behind, asks whether to pull the latest before running any checks. Diverged or ahead clones, offline runs, and non-interactive shells are left untouched, so it never blocks a run.

If you'd rather update manually:

```bash
cd ~/mini-moulinette && git pull
```


## Debugging

The error/success messages should be explicit enough. However, sometimes you'll hit a segmentation fault or your code won't compile.

### If your code doesn't compile

- Check the headers.
- Check whether the file contains a `main` — it shouldn't.
- Check whether your function name clashes with a standard library function.

### If you get a segmentation fault

Look at the test cases directly:

```bash
cd ~/mini-moulinette/mini-moul/tests
```

Every test file has the same name as the function/program it tests, e.g. `libft/libc/ft_isalpha.c` contains the tests for libc > `ft_isalpha`.

Test cases are declared as an array of structs, e.g. for `ft_isalpha`:

```c
typedef struct s_test
{
	char *desc;
	int c;
} t_test;

t_test tests[] = {
    {.desc = "ft_isalpha('a')", .c = 'a'},
    {.desc = "ft_isalpha('z')", .c = 'z'},
    {.desc = "ft_isalpha('`') ('a' - 1)", .c = '`'},
    {.desc = "ft_isalpha(200) (extended)", .c = 200},
    {.desc = "ft_isalpha(EOF)", .c = EOF},
    // ...
};
```


## Customizing

You can add more test cases in the files above. Note that you'll need to manage your customizations yourself when pulling updates.


## Contribution

Contributions are very welcome — especially new test cases for Common Core projects:

- **Tests**: If you spot an error in a test or have an idea for a new test case, open an issue or a pull request.
- **Code**: Pull requests for the runner itself are happily reviewed.
- **Materials**: I haven't started the Cursus yet (September!), so if you're already in the programme, sharing up-to-date subject materials would be greatly appreciated.
- **Feedback**: If you've used this fork and have suggestions, let me know via an issue.


## Authors

- Original author: [Khairul Haaziq](https://github.com/k11q)
- New Common Core fork: [gab-lee](https://github.com/gab-lee)


## Contributors

- This fork is co-created with Claude Code, which helps build and maintain the Common Core test suites


## License

MIT. Copyright 2026 gab-lee.
