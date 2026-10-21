#!/bin/bash

# The subject says not to interpret special characters it does not ask
# for, such as \ (backslash) and ; (semicolon). bash does interpret them, so
# these cases compare with a fixed text and only print a [!] when they
# differ: you may reasonably read the subject either way.
# Run by test.sh with the mini-moul directory as cwd; the student's
# project is at ../

source ./config.sh
source ./tests/minishell/ms_harness.sh

ms_prepare

# ms_literal <label> <expected stdout, printf format> <line>...
ms_literal()
{
	local label=$1 expected=$2
	shift 2
	printf '%s\n' "$@" > .ms_in
	printf "$expected" > .ms_ref.out
	ms_fixture
	ms_exec .ms_in .ms_got_raw "$MS_BIN"
	ms_clean out .ms_in .ms_got_raw.out > .ms_got.out
	if cmp -s .ms_ref.out .ms_got.out; then
		pc_ok "$label: treated as plain text"
	else
		pc_warn "$label: printed '$(ms_show .ms_got.out)'; the subject says not to interpret it, so plain text ('$(ms_show .ms_ref.out)') is expected"
	fi
}

ms_literal "a semicolon between two commands" 'a ; echo b\n' 'echo a ; echo b'
ms_literal "a semicolon inside a word" 'a;b\n' 'echo a;b'
ms_literal "a backslash inside a word" 'a\\b\n' 'echo a\b'
ms_literal "a trailing backslash" 'a\\\n' 'echo a\'
exit $pc_error
