#!/bin/bash

source config.sh

#utils
index=0
index2=0
assignment_data=NULL
test_data=NULL
test_error=NULL
test_name=NULL

#variables
checks=0
passed=0
marks=0
questions=0
dirname_found=0
break_score=0
score_false=0
available_assignments=""
result=""
has_bonus=0
bonus_passed=0
part_is_bonus=0
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )
dirname_found=0

# List a part's test files, honoring an optional 'order' file (one filename
# per line, subject order). Files missing from 'order' are appended after.
# A test is either a .c file (compiled and run) or a .sh check script
# (run directly; exit 0 is a PASS).
collect_tests()
{
    part_dir=$1
    if [ -f "$part_dir/order" ]; then
        while IFS= read -r f; do
            if [ -f "$part_dir/$f" ]; then
                printf '%s\n' "$part_dir/$f"
            else
                printf "${RED}file: %s is missing and cannot be compiled${DEFAULT}\n" "$f" >&2
            fi
        done < "$part_dir/order"
        for f in "$part_dir"/*.c "$part_dir"/*.sh; do
            [ -f "$f" ] || continue
            grep -qx "$(basename "$f")" "$part_dir/order" || printf '%s\n' "$f"
        done
    else
        ls "$part_dir"/*.c "$part_dir"/*.sh 2> /dev/null
    fi
}

# Compile each of the student's ft_*.c once into an object file. Tests
# declare prototypes (tests/<suite>/libft_proto.h) and link against these
# objects, so the student's code is never #include-d into a test. A file
# that doesn't compile keeps its error in $OBJ_DIR/<name>.err and its test
# reports it.
OBJ_DIR=""
student_objs=()
build_student_objects()
{
    OBJ_DIR="$SCRIPT_DIR/.student_objs"
    rm -rf "$OBJ_DIR"
    mkdir -p "$OBJ_DIR"
    student_objs=()
    for src in ../ft_*.c; do
        [ -f "$src" ] || continue
        name="$(basename "${src%.c}")"
        if cc -Wall -Werror -Wextra -c "$src" -o "$OBJ_DIR/$name.o" 2> "$OBJ_DIR/$name.err"; then
            rm -f "$OBJ_DIR/$name.err"
            student_objs+=("$OBJ_DIR/$name.o")
        fi
    done
}

# Path of the saved compile error for a function's source, if it failed
# to build (subject allows bonus files to carry a _bonus suffix).
student_compile_error()
{
    if [ -f "$OBJ_DIR/$1.err" ]; then
        printf '%s' "$OBJ_DIR/$1.err"
    elif [ -f "$OBJ_DIR/$1_bonus.err" ]; then
        printf '%s' "$OBJ_DIR/$1_bonus.err"
    fi
}

student_src_exists()
{
    [ -f "../$1.c" ] || [ -f "../$1_bonus.c" ]
}

# run_limited <status-file> <command...>
# Runs the command in its own process group and kills the whole group
# after $TEST_TIMEOUT seconds, so a hanging test (or anything it forked)
# can't stall the run. Writes "timeout" or the killing signal's name to
# <status-file> when the command didn't exit normally. perl, not
# timeout(1), because macOS has no timeout(1).
run_limited()
{
    perl -e '
        use Config;
        my ($limit, $status_file, @cmd) = @ARGV;
        my $pid = fork();
        die "fork: $!\n" unless defined $pid;
        if ($pid == 0) { setpgrp(0, 0); exec { $cmd[0] } @cmd; exit 127; }
        my $timed_out = 0;
        $SIG{ALRM} = sub { $timed_out = 1; kill "KILL", -$pid; };
        alarm $limit;
        while (waitpid($pid, 0) == -1 && $!{EINTR}) { }
        my $status = $?;
        alarm 0;
        my @names = split " ", $Config{sig_name};
        my $note = $timed_out ? "timeout"
            : ($status & 127) ? "SIG" . $names[$status & 127] : "";
        if ($note ne "") {
            open(my $fh, ">", $status_file) or die "$status_file: $!\n";
            print $fh $note;
            close $fh;
            exit 1;
        }
        exit($status >> 8);
    ' "$TEST_TIMEOUT" "$@"
}

# run_test <name> <command...>
# Runs one test under run_limited and prints its PASS/FAIL line. Exit 0 is
# a PASS; [!] lines in a passing test's output are known-strictness
# warnings and are still shown.
run_test()
{
    local name=$1
    shift
    local status_file=.test_status
    local output code note=""

    rm -f "$status_file"
    output="$(run_limited "$status_file" "$@" 2>&1 < /dev/null)"
    code=$?
    if [ -f "$status_file" ]; then
        case "$(cat "$status_file")" in
            timeout) note=" (timed out after ${TEST_TIMEOUT}s)" ;;
            *) note=" (crashed: $(cat "$status_file"))" ;;
        esac
        rm -f "$status_file"
    fi
    if [ $code -eq 0 ]; then
        passed=$((passed+1))
        printf " ${BG_GREEN}${BLACK}${BOLD} PASS ${DEFAULT} ${name}\n"
        case "$output" in
            *"[!]"*) printf '%s\n' "$output" | grep -F '[!]' ;;
        esac
    else
        [ $part_is_bonus -eq 0 ] && break_score=1
        score_false=1
        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${name}${RED}${note}${DEFAULT}\n"
        [ -n "$output" ] && printf '%s\n' "$output"
    fi
}

main()
{
    start_time=$(date +%s)
    #print_collected_files
    for dir in ./tests/* ; do
        dirname="$(basename "$dir")"
        available_assignments+="$dirname "
        
        if [ -d "$dir" ] && [ "$dirname" == "$1" ]; then
            dirname_found=1
            print_header
            printf "${GREEN} Generating test for ${1}...\n${DEFAULT}"
            space
            dirname_found=1
            index=0
            # An assignment whose subject has the student's Makefile build a
            # library (tests/<assignment>/library names it, e.g.
            # libftprintf.a) links its tests against that library, built by
            # the setup part, instead of compiling ../ft_*.c.
            library=""
            [ -f "$dir/library" ] && library="$(cat "$dir/library")"
            [ -z "$library" ] && build_student_objects

            # Run parts in subject order (setup, libc, additional), then any
            # other part, then bonus last
            exercise_dirs=""
            for part in setup libc additional; do
                [ -d "$dir/$part" ] && exercise_dirs+="$dir/$part "
            done
            for part in $dir/*; do
                case "$(basename "$part")" in
                    setup|libc|additional|bonus) continue ;;
                esac
                exercise_dirs+="$part "
            done
            [ -d "$dir/bonus" ] && exercise_dirs+="$dir/bonus "

            for assignment in $exercise_dirs; do
                [ -d "$assignment" ] || continue
                assignment_name="$(basename "$assignment")"
                score_false=0
                part_is_bonus=0
                if [ "$assignment_name" = "bonus" ]; then
                    part_is_bonus=1
                    has_bonus=1
                else
                    questions=$((questions+1))
                fi
                printf "${PURPLE}${BOLD} ${assignment_name}${DEFAULT}\n"
                test_files="$(collect_tests "$assignment")"

                bonus_build_failed=0
                if [ -n "$library" ] && [ $part_is_bonus -eq 1 ]; then
                    if ! make --no-print-directory -C .. bonus > make_bonus.tmp 2>&1; then
                        bonus_build_failed=1
                        printf "    ${RED}'make bonus' failed:${DEFAULT}\n"
                        sed 's/^/    /' make_bonus.tmp | head -15
                    fi
                    rm -f make_bonus.tmp
                fi

                for test in $test_files; do
                    checks=$((checks+1))

                    # Shell check scripts (setup part) run as-is, with the
                    # mini-moul directory as cwd and the project at ../
                    case "$test" in
                        *.sh)
                            run_test "$(basename "${test%.sh}")" bash "$test"
                            continue
                            ;;
                    esac

                    fn_name="$(basename "${test%.c}")"
                    fail_reason=""
                    fail_detail=""
                    if [ -n "$library" ]; then
                        link_inputs=("../$library")
                        if [ $bonus_build_failed -eq 1 ]; then
                            fail_reason="'make bonus' failed"
                        elif [ ! -f "../$library" ]; then
                            fail_reason="no $library in your project; see setup"
                        fi
                    else
                        link_inputs=("${student_objs[@]}")
                        fail_detail="$(student_compile_error "$fn_name")"
                        if [ -n "$fail_detail" ]; then
                            fail_reason="your ${fn_name}.c cannot compile"
                        elif ! student_src_exists "$fn_name"; then
                            fail_reason="no ${fn_name}.c found in your project"
                        fi
                    fi

                    if [ -n "$fail_reason" ]; then
                        [ $part_is_bonus -eq 0 ] && break_score=1
                        score_false=1
                        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name} ${RED}(${fail_reason})${DEFAULT}\n"
                        [ -n "$fail_detail" ] && sed 's/^/    /' "$fail_detail" | head -15
                    elif cc -Wall -Werror -Wextra -o "${test%.c}" "$test" "${link_inputs[@]}" 2> compile_error.tmp; then
                        run_test "$fn_name" "./${test%.c}"
                        rm -f "${test%.c}"
                    else
                        [ $part_is_bonus -eq 0 ] && break_score=1
                        score_false=1
                        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name} ${RED}(cannot compile)${DEFAULT}\n"
                        sed 's/^/    /' compile_error.tmp | head -15
                    fi
                    rm -f compile_error.tmp
                done
                print_test_result
                space
                ((index++))
            done
            break
        fi
    done
    
    if [ $dirname_found = 0 ]; then
        printf "${RED}Sorry. Tests for $1 isn't available yet. Consider contributing at Github.${DEFAULT}\n"
        printf "Available assignment tests: ${PURPLE}$available_assignments${DEFAULT}\n"
        exit 1
    fi
    [ -n "$OBJ_DIR" ] && rm -rf "$OBJ_DIR"
    print_footer
}

print_header()
{
    printf "${PINK}"
    space
    printf " ███▄ ▄███▓ ██▓ ███▄    █  ██▓\n"
    printf "▓██▒▀█▀ ██▒▓██▒ ██ ▀█   █ ▓██▒\n"
    printf "▓██    ▓██░▒██▒▓██  ▀█ ██▒▒██▒\n"
    printf "▒██    ▒██ ░██░▓██▒  ▐▌██▒░██░\n"
    printf "▒██▒   ░██▒░██░▒██░   ▓██░░██░\n"
    printf "░ ▒░   ░  ░░▓  ░ ▒░   ▒ ▒ ░▓  \n"
    printf "░  ░      ░ ▒ ░░ ░░   ░ ▒░ ▒ ░\n"
    printf "░      ░    ▒ ░   ░   ░ ░  ▒ ░\n"
    printf "       ░    ░           ░  ░  \n"
    printf "${DEFAULT}"
    printf "${BLUE}Mini moulinette ${DEFAULT}version ${VERSION}.\n"
    printf "${BLUE}Written by ${DEFAULT}gab-lee.\n"
    space
}

print_collected_files()
{
    printf "Collected files:\n"
    ls ../* | grep -v "../41test:*" | grep -v "../41test" | column
}

space()
{
    printf "\n"
}

print_test_result()
{
    if [ $index -gt 0 ]; then
        result+=", "
    fi
    if [ $score_false = 0 ]; then
        result+="${GREEN}$assignment_name: OK${DEFAULT}"
    else
        result+="${RED}$assignment_name: KO${DEFAULT}"
    fi
    if [ $part_is_bonus -eq 1 ]; then
        [ $score_false = 0 ] && bonus_passed=1
    elif [ $break_score = 0 ]; then
        marks=$((marks+1))
    fi
}

print_footer()
{
    printf "${PURPLE}-----------------------------------${DEFAULT}\n"
    space
    #printf "Total checks:  ""${GREEN}${passed} passed  ${DEFAULT} ""${checks} total"
    printf "Result:        ${result}\n"

    mandatory_percent=0
    [ $questions -gt 0 ] && mandatory_percent=$((100 * marks / questions))

    # Bonus is only ever graded once the mandatory part is a perfect 100,
    # mirroring 42's own moulinette. It's a flat +25, not proportional.
    final_score=$mandatory_percent
    denom=100
    if [ $has_bonus -eq 1 ] && [ $mandatory_percent -eq 100 ]; then
        denom=125
        if [ $bonus_passed -eq 1 ]; then
            final_score=125
            printf "Bonus:         ${GREEN}+25${DEFAULT}\n"
        else
            printf "Bonus:         ${RED}+0 (bonus checks failed)${DEFAULT}\n"
        fi
    elif [ $has_bonus -eq 1 ]; then
        printf "Bonus:         ${GREY}not evaluated (mandatory part isn't perfect)${DEFAULT}\n"
    fi

    if [ $final_score -ge 50 ]; then
        printf "Final score:   ""${GREEN}${final_score}/${denom}${DEFAULT}\n"
        printf "Status:        ""${GREEN}passed${DEFAULT}\n"
    else
        printf "Final score:   ""${RED}${final_score}/${denom}${DEFAULT}\n"
        printf "Status:        ""${RED}FAILED${DEFAULT}\n"
    fi
    end_time=$(date +%s)
    elapsed_time=$(expr $end_time - $start_time)
    printf "${GREY}Test completed. ${PINK}Total elapsed time: ${elapsed_time}s${DEFAULT}.\n"
    space
}

check_dependency()
{
    if ! command -v jq &> /dev/null; then
        printf "jq is not installed. To install:\n"
        printf "  Ubuntu/Debian:\n"
        printf "    sudo apt-get update\n"
        printf "    sudo apt-get install jq\n"
        printf "  macOS/Homebrew:\n"
        printf "    brew install jq\n"
    fi
}

#check_dependency
if [ "${1}" = "" ]; then
    printf "Please select a project. e.g. './test.sh libft'\n"
    exit 1
fi
main "$@"
printf "$DEFAULT"
exit
