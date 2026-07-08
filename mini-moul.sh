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

run_norminette() {
  if command -v norminette &> /dev/null; then
    norminette
  else
    echo "norminette not found, skipping norminette checks"
  fi
}

# Current directory must match a test suite under mini-moul/tests (archives excluded)
detect_assignment() {
  assignment=$(basename "$(pwd)")
  case "$assignment" in
    *"(archive)"*) return 1 ;;
  esac
  [ -d ~/mini-moulinette/mini-moul/tests/"$assignment" ]
}

if detect_assignment; then
  cp -R ~/mini-moulinette/mini-moul mini-moul
  run_norminette
  trap handle_sigint SIGINT
  cd mini-moul
  ./test.sh "$assignment"
  rm -R ../mini-moul
else
  printf "${RED}No test suite found for directory '$(basename "$(pwd)")'.${DEFAULT}\n"
  printf "${RED}Navigate to a project directory named after its test suite (e.g. libft) to run tests.${DEFAULT}\n"
  printf "${RED}Available test suites:${DEFAULT}\n"
  ls ~/mini-moulinette/mini-moul/tests | grep -v "(archive)"
fi

exit 1
