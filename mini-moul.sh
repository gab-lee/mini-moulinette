#!/bin/bash

source ~/mini-moulinette/mini-moul/config.sh
# assignment name
assignment=NULL

function handle_sigint {
  echo "${RED}Script aborted by user. Cleaning up..."
  rm -R ../mini-moul
  echo ""
  echo "${GREEN}Cleaning process done.${DEFAULT}"
  exit 1
}

# Fetches origin and, if the local mini-moulinette clone is behind, asks the
# user whether to fast-forward before running any checks.
check_for_updates() {
  local repo_dir=~/mini-moulinette

  if [ ! -d "$repo_dir/.git" ]; then
    return
  fi

  if ! git -C "$repo_dir" fetch --quiet origin 2>/dev/null; then
    return
  fi

  local upstream branch local_rev remote_rev base_rev
  upstream=$(git -C "$repo_dir" rev-parse --abbrev-ref --symbolic-full-name @{u} 2>/dev/null)
  if [ -z "$upstream" ]; then
    branch=$(git -C "$repo_dir" symbolic-ref --short HEAD 2>/dev/null)
    upstream="origin/$branch"
  fi

  local_rev=$(git -C "$repo_dir" rev-parse HEAD 2>/dev/null)
  remote_rev=$(git -C "$repo_dir" rev-parse "$upstream" 2>/dev/null)
  base_rev=$(git -C "$repo_dir" merge-base HEAD "$upstream" 2>/dev/null)

  # Nothing to compare, already up to date, or local has diverged/ahead: don't block.
  if [ -z "$local_rev" ] || [ -z "$remote_rev" ] || [ "$local_rev" = "$remote_rev" ] || [ "$local_rev" != "$base_rev" ]; then
    return
  fi

  printf "${BLUE}A new update for mini-moulinette is available.${DEFAULT}\n"

  if [ ! -t 0 ]; then
    printf "${GREY}Run 'git -C ~/mini-moulinette pull' to update. Continuing with the current version.${DEFAULT}\n"
    return
  fi

  read -r -p "$(printf "${BLUE}Update before running checks? [Y/n] ${DEFAULT}")" answer
  case "$answer" in
    [nN]*)
      printf "${GREY}Skipping update, running checks with the current version.${DEFAULT}\n"
      ;;
    *)
      if git -C "$repo_dir" merge --ff-only --quiet "$upstream" 2>/dev/null; then
        printf "${GREEN}mini-moulinette updated. Running checks with the latest version.${DEFAULT}\n"
      else
        printf "${RED}Update failed. Running checks with the current version.${DEFAULT}\n"
      fi
      ;;
  esac
}

# Current directory must match a test suite under mini-moul/tests, unless
# the suite is given explicitly as -<suite> (e.g. -libft)
detect_assignment() {
  if [ -z "$assignment_flag" ]; then
    assignment=$(basename "$(pwd)")
  else
    assignment="$assignment_flag"
  fi
  [ -d ~/mini-moulinette/mini-moul/tests/"$assignment" ]
}

print_usage() {
  printf "Usage: mini [-<suite>] [function ...]\n"
  printf "  mini                     run every test for the suite named after this folder\n"
  printf "  mini ft_strlen ft_split  run only these functions (the ft_ prefix is optional)\n"
  printf "  mini -libft strlen       pick the suite explicitly, for a folder with another name\n"
}

# Optional -<suite> first, then any number of function names
assignment_flag=""
case "$1" in
  -h|--help) print_usage; exit 0 ;;
  -*) assignment_flag="${1#-}"; shift ;;
esac

check_for_updates

if detect_assignment; then
  cp -R ~/mini-moulinette/mini-moul mini-moul
  trap handle_sigint SIGINT
  cd mini-moul
  ./test.sh "$assignment" "$@"
  rm -R ../mini-moul
else
  if [ -n "$assignment_flag" ]; then
    printf "${RED}No test suite named '$assignment_flag'.${DEFAULT}\n"
  else
    printf "${RED}No test suite found for directory '$(basename "$(pwd)")'.${DEFAULT}\n"
    printf "${RED}Navigate to a project directory named after its test suite (e.g. libft), or pass it as mini -libft.${DEFAULT}\n"
  fi
  printf "${RED}Available test suites:${DEFAULT}\n"
  ls ~/mini-moulinette/mini-moul/tests
fi

exit 1
