# Mini-moulinette — New Common Core

[![Buy Me A Beer](https://img.shields.io/badge/%F0%9F%8D%BA-Buy%20Me%20A%20Beer-FFDD00?style=for-the-badge)](https://buymeacoffee.com/gablee)

![mini-moulinette](mini-moulinette.jpg)

Mini-moulinette is a test runner for 42 assignments. It runs a full suite of automated tests over an assignment with a single command, so you can check your code thoroughly *before* submitting — instead of finding out the hard way at evaluation.

This repository is a fork that extends mini-moulinette to the **New Common Core** curriculum (Circles 0–6).


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

2. Create an alias for it.

- zsh:

```zsh
echo "alias mini='~/mini-moulinette/mini-moul.sh'" >> ~/.zshrc && source ~/.zshrc
```

- bash:

```bash
echo "alias mini='~/mini-moulinette/mini-moul.sh'" >> ~/.bashrc && source ~/.bashrc
```

3. Go to the project directory you want to test, e.g. `libft`:

```bash
cd libft
```

4. Run `mini` — it detects the project from the folder name:

```bash
mini
```

5. That's it — run it in every project directory where tests are provided. Have fun!


## Roadmap

The original project covers the piscine (C00–C08). The goal of this fork is to progressively add test suites for the Common Core, circle by circle:

- [x] Adapt the runner to detect Common Core project directories
- [x] **Circle 0** — Libft (Part 1, Part 2, Part 3 - linked list)
- [ ] **Circle 1** — ft_printf (mandatory + bonus) — first cut done
- [ ] **Circle 1** — get_next_line (mandatory + bonus) — first cut done
- [ ] **Circle 1** — push_swap (operation validity + sort check)
- [ ] **Circle 2** — minitalk / pipex
- [ ] **Circle 3** — philosophers (death timing / no-death scenarios), minishell (command comparison against bash)
- [ ] **Circle 4** — CPP Modules 00–04
- [ ] **Circle 5** — CPP Modules 05–09, webserv / ft_irc basic conformance tests

Projects that are graphical, system-administration or web-based (Born2beroot, so_long / FdF / fract-ol, NetPractice, cub3D / miniRT, inception, ft_transcendence) don't lend themselves well to automated unit testing, so they are out of scope for now — the table below still lists them for completeness.


## Coverage Status

| Circle | Project | What it is | Tests cover | Coverage |
| :----: | :------ | :--------- | :---------- | :------: |
| 0 | Libft | Your own C library: libc functions, string and memory helpers, and a linked list, reused in later projects. | Part 1 (libc), Part 2 (additional), Part 3 (linked list), Makefile | Complete |
| 1 | ft_printf | Recode `printf()` as the `libftprintf.a` library, using variadic arguments. | Makefile, `cspdiuxX%` conversions, bonus flags `-0.# +` and width | First cut |
| 1 | get_next_line | A function that returns one line per call from a file descriptor, using static variables. | Every `BUFFER_SIZE` from 1 to 10000000, files, stdin and pipes, invalid fds, leaks, multiple-fd bonus | First cut |
| 1 | push_swap | Sort integers with two stacks and a limited set of operations, in as few operations as possible. | Operation validity, sort check, operation count | Planned |
| 2 | Born2beroot | Set up a hardened Linux server in a virtual machine (partitions, sudo and password policy, firewall, SSH, monitoring script). | — | Out of scope (VM / sysadmin) |
| 2 | pipex *(or minitalk)* | Reproduce the shell pipeline `< file1 cmd1 \| cmd2 > file2` with `pipe`, `fork`, `dup2` and `execve`. | Output and exit status against the real shell | Planned |
| 2 | minitalk *(or pipex)* | A client and server that exchange strings using only the `SIGUSR1` and `SIGUSR2` signals. | Message delivery, including Unicode and long strings | Planned |
| 2 | so_long *(or FdF, fract-ol)* | A small 2D game with the MiniLibX: collect every item and reach the exit on a map read from a `.ber` file. | — | Out of scope (graphical) |
| 2 | FdF *(or so_long, fract-ol)* | Render a height map as a 3D wireframe landscape with the MiniLibX. | — | Out of scope (graphical) |
| 2 | fract-ol *(or so_long, FdF)* | Explore the Mandelbrot and Julia fractals with the MiniLibX. | — | Out of scope (graphical) |
| 3 | Philosophers | The dining philosophers problem with threads and mutexes (bonus: processes and semaphores). | Death timing, no-death scenarios | Planned |
| 3 | minishell | A small bash-like shell: prompt, history, pipes, redirections, heredoc, environment variables, builtins and signals. | Output comparison against bash | Planned |
| 4 | NetPractice | Configure small IP networks (addressing, subnet masks, routing) in a browser-based exercise. | — | Out of scope (web exercise) |
| 4 | cub3D *(or miniRT)* | A first-person raycasting maze in the style of Wolfenstein 3D, with the MiniLibX. | — | Out of scope (graphical) |
| 4 | miniRT *(or cub3D)* | A basic ray tracer that renders spheres, planes and cylinders with lighting. | — | Out of scope (graphical) |
| 4 | CPP Module 00 | C++ basics: namespaces, classes, member functions, `iostream`. | Per-exercise behaviour tests | Planned |
| 4 | CPP Module 01 | Memory allocation, pointers to members, references, `switch`. | Per-exercise behaviour tests | Planned |
| 4 | CPP Module 02 | Ad-hoc polymorphism, operator overloading, the Orthodox Canonical class form. | Per-exercise behaviour tests | Planned |
| 4 | CPP Module 03 | Inheritance. | Per-exercise behaviour tests | Planned |
| 4 | CPP Module 04 | Subtype polymorphism, abstract classes, interfaces. | Per-exercise behaviour tests | Planned |
| 5 | CPP Module 05 | Repetition and exceptions. | Per-exercise behaviour tests | Planned |
| 5 | CPP Module 06 | C++ casts. | Per-exercise behaviour tests | Planned |
| 5 | CPP Module 07 | Templates. | Per-exercise behaviour tests | Planned |
| 5 | CPP Module 08 | Templated containers, iterators, algorithms. | Per-exercise behaviour tests | Planned |
| 5 | CPP Module 09 | The Standard Template Library. | Per-exercise behaviour tests | Planned |
| 5 | Inception | A small Docker Compose infrastructure (NGINX with TLS, WordPress, MariaDB) inside a virtual machine. | — | Out of scope (Docker infra) |
| 5 | webserv *(or ft_irc)* | An HTTP server in C++98 with non-blocking I/O, a configuration file and CGI. | Basic HTTP conformance | Planned |
| 5 | ft_irc *(or webserv)* | An IRC server in C++98 that works with a real IRC client. | Basic IRC protocol conformance | Planned |
| 6 | ft_transcendence | A full-stack web application built around a real-time multiplayer Pong game. | — | Out of scope (web project) |
| Outer | libasm | Rewrite a few libc functions (`strlen`, `strcpy`, `strcmp`, `write`, `read`, `strdup`) in x86-64 assembly. | Output, return values and `errno` against libc | Planned |

*(or …)* marks a choice: at that circle you complete only one of the listed alternatives.

> [!NOTE]
> **Known strictness exceptions.** Some checks are stricter than 42's moulinette, or cannot tell a real mistake from something the compiler did. These cases print a yellow `[!]` warning instead of failing:
> - `ft_strchr` / `ft_strrchr` searching for an extended character (e.g. 233): implementations that compare as `unsigned char` return NULL where libc finds the byte.
> - `ft_calloc(SIZE_MAX, SIZE_MAX)`: the real calloc returns NULL on `count * size` overflow, but passing this is not required.
> - get_next_line reading further ahead than it needs to (for example one extra `read()` per call): the subject says to read as little as possible, but only reading the whole file on the first call fails.
> - get_next_line calling `memset`, `memcpy`, `memmove` or `bzero`: the compiler can generate these calls on its own (for example to zero an array), so they are not failed, but calling them yourself is forbidden.

> [!NOTE]
> **ft_printf** is tested the way 42's moulinette does it: `setup` runs your `make`, and every test links against the `libftprintf.a` it builds, so sources in subfolders or a bundled libft are fine. The `bonus` part runs `make bonus` first. Output and return values are compared against the real `printf` on your machine, so `%p` of `NULL` expects `(nil)` on Linux and `0x0` on macOS. The subject lets you do only some bonus flags, but the `bonus` part currently expects all of them.

> [!NOTE]
> **get_next_line** is compiled and run with every `BUFFER_SIZE` in 1, 2, 5, 42, 9999 and 10000000, and once without `-D BUFFER_SIZE` (the subject requires both). Each case runs on its own with a 5-second limit, so an infinite loop, a crash or a `read()` that waits forever fails that one case instead of hanging the run; a failure shows the case, the buffer size, the line expected and the line returned. The tests also check for memory leaks after `get_next_line` returns `NULL`, reject global variables and functions other than `read`, `malloc` and `free`, and fail an implementation that reads the whole file before returning the first line.


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
- **Materials**: If your campus's subject versions differ from what the tests expect, sharing them would be greatly appreciated.
- **Feedback**: If you've used this fork and have suggestions, let me know via an issue.


## Authors

- Original author: [Khairul Haaziq](https://github.com/k11q)
- New Common Core fork: [gab-lee](https://github.com/gab-lee)

All credit for the original mini-moulinette goes to **[Khairul Haaziq](https://github.com/k11q)** — the original project lives at [k11q/mini-moulinette](https://github.com/k11q/mini-moulinette).

This tool was incredibly useful during my piscine: it saved me countless hours of waiting for evaluations only to fail on silly mistakes. This fork exists because I want the same safety net while going through the Common Core. Thank you, Khairul! 🙏


## Contributors

- This fork is co-created with Claude Code, which helps build and maintain the Common Core test suites


## License

MIT. Copyright 2026 gab-lee.
