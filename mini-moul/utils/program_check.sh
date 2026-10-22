#!/bin/bash

# Helpers shared by the suites that grade a whole program built by the
# student's Makefile (push_swap, philo, minishell). Sourced by the .sh test
# scripts with the mini-moul directory as cwd and config.sh already sourced.
#
# Output helpers number every line like the other setup scripts do:
#   pc_ok / pc_bad <text>   a passing / failing check (pc_bad sets pc_error)
#   pc_detail <text>        extra red line under a failure
#   pc_warn <text>          yellow [!] line, shown even when the test passes
# A script ends with `exit $pc_error`.

pc_i=1
pc_error=0

pc_ok()
{
	printf "  ${GREEN}${CHECKMARK}${GREY} [%s] %s${DEFAULT}\n" "$pc_i" "$1"
	pc_i=$((pc_i+1))
}

pc_bad()
{
	printf "    ${RED}[%s] %s${DEFAULT}\n" "$pc_i" "$1"
	pc_i=$((pc_i+1))
	pc_error=1
}

pc_detail()
{
	printf "    ${RED}    %s${DEFAULT}\n" "$1"
}

pc_warn()
{
	printf "  ${YELLOW}[!] %s${DEFAULT}\n" "$1"
}

# pc_limited <seconds> <command...>
# Runs the command and kills it after <seconds>. The exit status is the
# command's (142 on a timeout); pc_exit, pc_signal and pc_timeout tell an
# exit status of 255 from a crash.
pc_exit=0
pc_signal=0
pc_timeout=0
pc_limited()
{
	local seconds=$1
	shift
	perl ./utils/limited.pl "$seconds" .pc_status "$@"
	local code=$?
	read -r pc_exit pc_signal pc_timeout < .pc_status
	return $code
}

# pc_check_makefile <dir> <binary> <rule...>
# The Makefile the subject requires: it exists in <dir>, uses -Wall -Wextra
# -Werror, builds <binary> without errors and without relinking, and has
# every rule listed. A dry run (make -n) tells whether a rule exists, so
# nothing is cleaned in the student's project.
pc_check_makefile()
{
	local dir=$1 binary=$2 rule
	shift 2
	local make_out=.make_out

	if [ ! -f "$dir/Makefile" ]; then
		pc_bad "Makefile is missing at the root of your project"
		return 1
	fi
	pc_ok "Makefile exists"

	if grep -q -- "-Wall" "$dir/Makefile" && grep -q -- "-Wextra" "$dir/Makefile" \
		&& grep -q -- "-Werror" "$dir/Makefile"; then
		pc_ok "Makefile uses -Wall -Wextra -Werror"
	else
		pc_bad "Makefile must compile with -Wall -Wextra -Werror"
	fi

	if grep -Eq '^[[:space:]]*CC[[:space:]]*[:?]?=[[:space:]]*(gcc|clang|g\+\+|c\+\+)' "$dir/Makefile"; then
		pc_bad "Makefile must compile with cc (it sets CC to another compiler)"
	else
		pc_ok "Makefile compiles with cc"
	fi

	if make -C "$dir" > "$make_out" 2>&1; then
		pc_ok "make runs without error"
		if [ -f "$dir/$binary" ]; then
			pc_ok "make produced $binary"
		else
			pc_bad "make succeeded but did not produce $binary"
		fi
	else
		pc_bad "make failed:"
		sed 's/^/    /' "$make_out" | head -15
	fi

	if [ -f "$dir/$binary" ]; then
		if make -C "$dir" -n 2> /dev/null | grep -Eq '(^|[[:space:]/])(ar|cc|gcc|clang)[[:space:]]'; then
			pc_bad "make relinks: a second make with nothing changed still runs:"
			make -C "$dir" -n 2> /dev/null | grep -E '(^|[[:space:]/])(ar|cc|gcc|clang)[[:space:]]' \
				| sed 's/^/    /' | head -5
		else
			pc_ok "make does not relink when nothing changed"
		fi
	fi

	for rule in "$@"; do
		if make -C "$dir" -n "$rule" > /dev/null 2>&1; then
			pc_ok "rule '$rule' exists"
		else
			pc_bad "rule '$rule' is missing"
		fi
	done
	rm -f "$make_out"
	return $pc_error
}

# pc_ensure_built <dir> <binary> [make target]
# Builds <binary> with the student's Makefile when it is not there yet.
# Fails the calling script when it cannot be built.
pc_ensure_built()
{
	local dir=$1 binary=$2 target=$3
	[ -x "$dir/$binary" ] && return 0
	make -C "$dir" $target > .make_out 2>&1
	if [ ! -x "$dir/$binary" ]; then
		printf "    ${RED}%s cannot be built, so there is nothing to run:${DEFAULT}\n" "$binary"
		sed 's/^/    /' .make_out | head -10
		rm -f .make_out
		return 1
	fi
	rm -f .make_out
	return 0
}

# pc_symbols <binary-or-object...>
# "type name" for every symbol, with the version suffix and, on macOS, the
# leading underscore removed.
pc_symbols()
{
	local symbols
	symbols="$(nm "$@" 2> /dev/null | awk 'NF >= 2 { print $(NF-1), $NF }' | sed 's/@.*//')"
	if [ "$(uname)" = "Darwin" ]; then
		symbols="$(printf '%s\n' "$symbols" | sed 's/ _/ /')"
	fi
	printf '%s\n' "$symbols"
}

# pc_check_forbidden <dir> <binary> <max globals> <allowed functions> [warn-only functions]
# Fails when the program calls a function that leaves the student's code
# (libft included) and is not in <allowed functions> (space separated), and
# when the objects found under <dir> define more than <max globals> global
# variables (a static outside a function counts; the compiler names a
# function's static "name.N", so a dot tells it from a file-scope one).
# Functions the compiler emits on its own (memset, puts, ...) in
# [warn-only functions] only print a [!].
pc_check_forbidden()
{
	local dir=$1 binary=$2 max_globals=$3 allowed=$4 warn_only=$5
	local calls bad warned allowed_re warn_re objs symbols data globals count

	if [ ! -f "$dir/$binary" ]; then
		pc_bad "cannot check: $binary was not built"
		return 1
	fi
	allowed_re="^($(printf '%s' "$allowed" | tr ' ' '|')|__.*|_ITM_.*|dyld_stub_binder|environ|stdin|stdout|stderr)\$"
	warn_re="^($(printf '%s' "$warn_only" | tr ' ' '|'))\$"
	calls="$(pc_symbols "$dir/$binary" | awk '$1 == "U" { print $2 }' | sort -u)"
	warned=""
	[ -n "$warn_only" ] && warned="$(printf '%s\n' "$calls" | grep -E "$warn_re" | paste -sd ' ' -)"
	bad="$(printf '%s\n' "$calls" | grep -vE "$allowed_re" | { [ -n "$warn_only" ] && grep -vE "$warn_re" || cat; } | paste -sd ' ' -)"
	if [ -n "$bad" ]; then
		pc_bad "uses functions the subject does not allow: $bad"
	else
		pc_ok "only the functions the subject allows are used"
	fi
	[ -n "$warned" ] && pc_warn "your program calls $warned: fine if the compiler generated it, but the subject does not allow it"

	objs="$(find "$dir" -name '*.o' -not -path '*/mini-moul/*' -not -path '*/.git/*' 2> /dev/null)"
	if [ -z "$objs" ]; then
		pc_warn "no object files found after make, so global variables could not be checked"
		return $pc_error
	fi
	# shellcheck disable=SC2086
	symbols="$(pc_symbols $objs)"
	data="$(printf '%s\n' "$symbols" | awk '$1 ~ /^[BbCDdGRrSsV]$/' \
		| grep -vE ' (ltmp[0-9]+|[lL]_.*|\..*|environ)$')"
	globals="$(printf '%s\n' "$data" | awk '$1 ~ /^[BCDGRSV]$/ || $2 !~ /\./ { print $2 }' \
		| sort -u | paste -sd ' ' -)"
	count="$(printf '%s' "$globals" | wc -w | tr -d ' ')"
	if [ "$count" -le "$max_globals" ]; then
		if [ "$max_globals" -eq 0 ]; then
			pc_ok "no global variables"
		else
			pc_ok "at most $max_globals global variable"
		fi
	elif [ "$max_globals" -eq 0 ]; then
		pc_bad "global variables are forbidden (a static outside a function is one too): $globals"
	else
		pc_bad "$count global variables ($globals); the subject allows $max_globals"
	fi
	return $pc_error
}

# pc_check_readme <README.md path>
# The subject requires a README.md at the root of the repository whose
# first line is italic and reads "This project has been created as part of
# the 42 curriculum by <login>...", with Description, Instructions and
# Resources sections. The explanation of how AI was used cannot be checked.
pc_check_readme()
{
	local readme=$1 first section

	if [ ! -f "$readme" ]; then
		pc_bad "README.md is missing at the root of your repository (the subject requires one)"
		return 1
	fi
	pc_ok "README.md exists"
	first="$(grep -v '^[[:space:]]*$' "$readme" | head -1 | tr -d '\r')"
	if printf '%s\n' "$first" | grep -qE '^([*]This project has been created as part of the 42 curriculum by .+[*]|_This project has been created as part of the 42 curriculum by .+_)[[:space:]]*$' \
		&& ! printf '%s\n' "$first" | grep -q '^[*][*]'; then
		pc_ok "first line is the italic \"This project has been created as part of the 42 curriculum by ...\""
	else
		pc_bad "the first line must be italic and read: *This project has been created as part of the 42 curriculum by <login>*"
		pc_detail "yours: $first"
	fi
	for section in Description Instructions Resources; do
		if grep -qiE "^#{1,6}[[:space:]]*${section}" "$readme"; then
			pc_ok "has the \"$section\" section"
		else
			pc_bad "no \"$section\" section (a markdown heading such as ## $section)"
		fi
	done
	return $pc_error
}

# pc_check_global_size <dir> <max bytes>
# The only global variable minishell may have stores a signal number, so a
# global bigger than <max bytes> (a structure) fails. Needs nm -S, which
# macOS does not have: the check is skipped there with a [!].
pc_check_global_size()
{
	local dir=$1 max=$2 objs sizes name size dec worst=""
	objs="$(find "$dir" -name '*.o' -not -path '*/mini-moul/*' -not -path '*/.git/*' 2> /dev/null)"
	[ -z "$objs" ] && return 0
	# shellcheck disable=SC2086
	sizes="$(nm -S $objs 2> /dev/null | awk 'NF == 4 && ($3 ~ /^[BCDGRSV]$/ || ($3 ~ /^[bdrs]$/ && $4 !~ /\./)) && $4 !~ /^(environ|\..*)$/ { print $2, $4 }')"
	if [ -z "$sizes" ]; then
		if ! nm -S $objs 2> /dev/null | awk 'NF == 4 { found = 1 } END { exit !found }'; then
			pc_warn "nm cannot print symbol sizes here, so the size of the global variable was not checked"
		fi
		return 0
	fi
	while read -r size name; do
		dec=$((16#$size))
		if [ "$dec" -gt "$max" ]; then
			worst="$worst $name ($dec bytes)"
		fi
	done <<< "$sizes"
	if [ -n "$worst" ]; then
		pc_bad "the global variable may only hold a signal number, but these are larger than $max bytes:$worst"
	else
		pc_ok "the global variable is small enough to hold only a signal number"
	fi
}

# pc_instrumented_build <kind> <project dir> <sanitizer flags> <marker symbol> <binary> [make target...]
# Builds a copy of the project with the sanitizer (CC is overridden on the
# make command line, so it reaches a libft sub-make too) and leaves it in
# .instr_<kind>/<project name>. Succeeds when <binary> exists in the copy
# and really carries the sanitizer (<marker symbol>, e.g. __asan_init); a
# Makefile that hardcodes its compiler produces a plain binary instead,
# which is reported as a failure so the caller can fall back and warn.
# The result is cached for the rest of the run.
pc_instrumented_build()
{
	local kind=$1 src=$2 flags=$3 marker=$4 binary=$5 name root dest target
	shift 5
	name="$(basename "$(cd "$src" && pwd)")"
	root=".instr_$kind"
	dest="$root/$name"
	if [ -f "$root/$name.status" ]; then
		[ "$(cat "$root/$name.status")" = "ok" ]
		return $?
	fi
	mkdir -p "$root"
	rm -rf "$dest"
	mkdir -p "$dest"
	tar -C "$src" --exclude=./mini-moul --exclude=./.git -cf - . | tar -C "$dest" -xf -
	make -C "$dest" fclean > /dev/null 2>&1
	if [ $# -eq 0 ]; then
		set -- all
	fi
	for target in "$@"; do
		make -C "$dest" "$target" CC="cc $flags" > "$root/$name.log" 2>&1
	done
	if [ -x "$dest/$binary" ] && nm "$dest/$binary" 2> /dev/null | grep -q "$marker"; then
		printf 'ok' > "$root/$name.status"
		return 0
	fi
	printf 'failed' > "$root/$name.status"
	return 1
}

# pc_sanitizer_report <file>
# Succeeds when the file holds an AddressSanitizer, LeakSanitizer or
# ThreadSanitizer report.
pc_sanitizer_report()
{
	grep -qE 'ERROR: (AddressSanitizer|LeakSanitizer)|WARNING: ThreadSanitizer' "$1"
}
