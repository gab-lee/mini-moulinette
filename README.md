# Mini-moulinette — New Common Core

[![Buy Me A Coffee](https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png)](https://buymeacoffee.com/gablee)

![mini-moulinette](mini-moulinette.jpg)

Mini-moulinette is a test runner for 42 assignments. It runs a full suite of automated tests over an assignment with a single command, so you can check your code thoroughly *before* submitting — instead of finding out the hard way at evaluation.

This repository is a fork that extends mini-moulinette to the **New Common Core** curriculum (Circles 0–6).


## Credits

All credit for the original mini-moulinette goes to **[Khairul Haaziq](https://github.com/k11q)** — the original project lives at [k11q/mini-moulinette](https://github.com/k11q/mini-moulinette).

This tool was incredibly useful during my piscine: it saved me countless hours of waiting for evaluations only to fail on silly mistakes. This fork exists because I want the same safety net while going through the Common Core. Thank you, Khairul! 🙏


## How Does It Work?

![screenshot](screenshot.jpg)

- Mini-moulinette runs through all the test cases for an assignment automatically and checks that the expected conditions are met.
- It then prints a per-exercise pass/fail summary.
- Scoring follows 42's practice: if an earlier exercise fails, the following ones don't count.


## Get Started

> ***Warning***
> Mini-moulinette is not 100% accurate — the tests may not cover every edge case the real moulinette does. Use it as a safety net, not a guarantee.

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

> [!NOTE]
> **A caveat:** I haven't started the 42 Cursus yet — I'll only be starting in September. A lot of the project list and scope below is pieced together from PDFs found online, so it may not match the current curriculum exactly. If you're already in the programme and would like to contribute, access to up-to-date materials would also be greatly appreciated!

- [x] Adapt the runner to detect Common Core project directories
- [x] **Circle 0** — Libft (Part 1, Part 2, bonus list functions)
- [ ] **Circle 1** — ft_printf, get_next_line (mandatory + bonus) — targeting end of August
- [ ] **Circle 2** — push_swap (operation validity + sort check), minitalk / pipex
- [ ] **Circle 3** — philosophers (death timing / no-death scenarios), minishell (command comparison against bash)
- [ ] **Circle 4** — CPP Modules 00–04
- [ ] **Circle 5** — CPP Modules 05–09, webserv / ft_irc basic conformance tests

Projects that are graphical, system-administration or web-based (Born2beroot, so_long / FdF / fract-ol, NetPractice, cub3D / miniRT, inception, ft_transcendence) don't lend themselves well to automated unit testing, so they are out of scope for now — the table below still lists them for completeness.


## Coverage Status

> Looking for **piscine** tests (C00–C08)? This fork focuses on the New Common Core — head over to the original [k11q/mini-moulinette](https://github.com/k11q/mini-moulinette) for the piscine test suites.

| Circle | Project                    | Exercises / Parts to cover                 | Coverage        | Cross-tested against 42 submissions |
| :----: | :------------------------- | :----------------------------------------- | :-------------: | :---------------------------------- |
| 0      | Libft                      | Part 1 (libc), Part 2 (additional), bonus  | Complete        | [Mia Combeau](https://github.com/mcombeau/libft/tree/main) |
| 1      | ft_printf                  | Mandatory conversions + bonus flags        | Planned (targeting end of Aug) | —                     |
| 1      | get_next_line              | Mandatory + multiple-fd bonus              | Planned (targeting end of Aug) | —                     |
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
> **Known strictness exceptions.** The libc tests compare against the *real* libc, which is stricter than 42's moulinette in a few corners. These cases print a yellow `[!]` warning instead of failing the function:
> - `ft_strchr` / `ft_strrchr` searching for an extended character (e.g. 233): implementations that compare as `unsigned char` return NULL where libc finds the byte.
> - `ft_calloc(SIZE_MAX, SIZE_MAX)`: the real calloc returns NULL on `count * size` overflow, but passing this is not required.


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
