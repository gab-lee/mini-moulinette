# Defines the `mini` command and its tab completion for bash and zsh.
# Add this line to ~/.zshrc or ~/.bashrc:
#   source ~/mini-moulinette/mini.sh
#
#   mini                   whole suite for the folder you're in
#   mini strlen split      only these functions (ft_ prefix optional)
#   mini -libft strlen     pick the suite explicitly
# Tab completes suites after `-` and the suite's function names.

unalias mini 2> /dev/null
mini()
{
	~/mini-moulinette/mini-moul.sh "$@"
}

_mini_complete()
{
	local cur tests suite words name
	cur="${COMP_WORDS[COMP_CWORD]}"
	tests=~/mini-moulinette/mini-moul/tests
	suite="$(basename "$PWD")"
	case "${COMP_WORDS[1]}" in
		-*) [ "$COMP_CWORD" -gt 1 ] && suite="${COMP_WORDS[1]#-}" ;;
	esac
	words=""
	if [ "$COMP_CWORD" -eq 1 ]; then
		for name in $(ls "$tests" 2> /dev/null); do
			words="$words -$name"
		done
	fi
	if [ -d "$tests/$suite" ]; then
		for name in $(find "$tests/$suite" -mindepth 2 -maxdepth 2 -name '*.c' 2> /dev/null); do
			name="$(basename "${name%.c}")"
			# Offer ft_strlen once the user has typed an f, strlen otherwise
			case "$cur" in
				f*) words="$words $name" ;;
				*) words="$words ${name#ft_}" ;;
			esac
		done
	fi
	COMPREPLY=($(compgen -W "$words" -- "$cur"))
}

if [ -n "$ZSH_VERSION" ]; then
	if ! (( $+functions[compdef] )); then
		autoload -Uz compinit && compinit
	fi
	autoload -Uz bashcompinit && bashcompinit
fi
complete -F _mini_complete mini
