# Defines the `mini` command and its tab completion for bash and zsh.
# Add this line to ~/.zshrc or ~/.bashrc:
#   source ~/mini-moulinette/mini.sh
#
#   mini                   whole suite for the folder you're in
#   mini strlen split      only these functions (ft_ prefix optional)
#   mini -libft strlen     pick the suite explicitly
#   mini --show strlen     also print every case of a passing test
#   mini --try strlen "hi" call one function with your own arguments
# Tab completes the options, suites after `-` and the suite's function
# names (only the function after --try, not its arguments).

unalias mini 2> /dev/null
mini()
{
	~/mini-moulinette/mini-moul.sh "$@"
}

_mini_complete()
{
	local cur tests suite words name w i options try
	cur="${COMP_WORDS[COMP_CWORD]}"
	tests=~/mini-moulinette/mini-moul/tests
	suite="$(basename "$PWD")"
	options=1
	try=0
	i=1
	while [ "$i" -lt "$COMP_CWORD" ]; do
		w="${COMP_WORDS[$i]}"
		case "$w" in
			--try) [ "$options" -eq 1 ] && try=1 ;;
			--*) ;;
			-*) [ "$options" -eq 1 ] && suite="${w#-}" ;;
			*)
				# After --try <function>, the rest are its arguments
				if [ "$try" -eq 1 ]; then
					COMPREPLY=()
					return
				fi
				options=0
				;;
		esac
		i=$((i + 1))
	done
	words=""
	if [ "$options" -eq 1 ]; then
		words="--show --try --help"
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
