#!/bin/bash

source config.sh

# Compiler flags shared by every compile/link step. AddressSanitizer is
# always on: it catches memory-safety bugs (heap-buffer-overflow,
# use-after-free) that pure output/return-value comparison can't see, and
# on Linux its LeakSanitizer also fails a test that leaks. LeakSanitizer
# isn't supported on macOS/arm64, so there only memory-safety bugs are
# caught. There is no opt-out: if the compiler can't build with
# -fsanitize=address, the run stops instead of grading without it.
CC_FLAGS="-Wall -Werror -Wextra -fsanitize=address -g"
asan_probe()
{
    probe_dir="$(mktemp -d)"
    printf 'int main(void)\n{\n\treturn (0);\n}\n' > "$probe_dir/p.c"
    cc -fsanitize=address -o "$probe_dir/p" "$probe_dir/p.c" > /dev/null 2>&1 \
        && "$probe_dir/p" > /dev/null 2>&1
    status=$?
    rm -rf "$probe_dir"
    return $status
}
if ! asan_probe; then
    printf "${RED}AddressSanitizer is unavailable: cc cannot build and run a program with -fsanitize=address.${DEFAULT}\n"
    printf "${RED}Mini always tests with AddressSanitizer; install a compiler that supports it and run again.${DEFAULT}\n"
    exit 1
fi
# Oversized requests (e.g. ft_calloc(INT_MAX, INT_MAX)) must return NULL
# like the real allocator instead of aborting the test. A sanitizer report
# exits with 86, so a harness that forks per case (get_next_line) can
# tell it from an ordinary failure; test.sh itself
# detects reports from stderr. Appended after any user ASAN_OPTIONS so
# they can't switch these off (later options win).
asan_opts="allocator_may_return_null=1:exitcode=86"
[ "$(uname -s)" = "Linux" ] && asan_opts="$asan_opts:detect_leaks=1"
export ASAN_OPTIONS="${ASAN_OPTIONS:+$ASAN_OPTIONS:}${asan_opts}"

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

# Compile each of the student's subject functions once into an object
# file. Only sources named after a test file in the suite (<fn>.c or
# <fn>_bonus.c) are built, so extra accessory files in the project (other
# ft_*.c helpers, a main, ...) are ignored. Tests declare prototypes
# (tests/<suite>/libft_proto.h) and link against these objects, so the
# student's code is never #include-d into a test. A file that doesn't
# compile keeps its error in $OBJ_DIR/<name>.err and its test reports it.
OBJ_DIR=""
student_objs=()
build_student_objects()
{
    suite_dir=$1
    OBJ_DIR="$SCRIPT_DIR/.student_objs"
    rm -rf "$OBJ_DIR"
    mkdir -p "$OBJ_DIR"
    student_objs=()
    for test_src in "$suite_dir"/*/*.c; do
        [ -f "$test_src" ] || continue
        fn="$(basename "${test_src%.c}")"
        for src in "../$fn.c" "../${fn}_bonus.c"; do
            [ -f "$src" ] || continue
            name="$(basename "${src%.c}")"
            if cc $CC_FLAGS -c "$src" -o "$OBJ_DIR/$name.o" 2> "$OBJ_DIR/$name.err"; then
                rm -f "$OBJ_DIR/$name.err"
                student_objs+=("$OBJ_DIR/$name.o")
            fi
        done
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

# Kind of AddressSanitizer/LeakSanitizer error in a test's stderr ("memory
# leak", "heap-buffer-overflow", ...), or nothing if there is none.
memory_error_kind()
{
    if grep -q 'ERROR: LeakSanitizer' "$1"; then
        printf 'memory leak'
    else
        sed -n 's/.*ERROR: AddressSanitizer: \([A-Za-z-]*\).*/\1/p' "$1" | head -1
    fi
}

# One-line description of a sanitizer report: the SUMMARY text plus the
# student source lines (frames in ../*.c) involved, e.g.
# "24 byte(s) leaked in 6 allocation(s), at ft_strjoin.c:3".
memory_error_detail()
{
    local summary sites
    summary="$(sed -n 's/^SUMMARY: AddressSanitizer: //p' "$1" | head -1)"
    case "$summary" in
        *leaked*) summary="${summary%.}" ;;
        *) summary="$(memory_error_kind "$1")" ;;
    esac
    sites="$(sed -n 's/^ *#[0-9]* 0x[0-9a-f]* in [^ ]* \.\.\/\([^ /]*\.c:[0-9]*\).*/\1/p' "$1" \
        | awk '!seen[$0]++' | paste -sd ',' - | sed 's/,/, /g')"
    if [ -n "$sites" ]; then
        printf '%s, at %s' "$summary" "$sites"
    else
        printf '%s' "$summary"
    fi
}

# run_limited <status-file> <command...>
# Runs the command in its own process group and kills the whole group
# after $TEST_TIMEOUT seconds or on Ctrl-C, so a hanging test (or anything
# it forked) can't stall the run. Writes "timeout" or the killing signal's
# name to <status-file> when the command didn't exit normally. perl, not
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
        # The test is in its own process group, so Ctrl-C only reaches us
        $SIG{$_} = sub { kill "KILL", -$pid; exit 130; } for qw(INT TERM HUP);
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
# Runs one test (a .c test binary or a .sh check script) under run_limited
# and prints its PASS/FAIL line. Exit 0 is a PASS; [!] lines in a passing
# test's output are known-strictness warnings and are still shown. An
# AddressSanitizer/LeakSanitizer report on stderr is a Memory fail, and a
# timeout or crash signal is named on the FAIL line.
run_test()
{
    local name=$1
    shift
    local status_file=.test_status
    local output code note="" memory_error test_err

    rm -f "$status_file"
    output="$(run_limited "$status_file" "$@" 2> asan_report.tmp < /dev/null)"
    code=$?
    if [ -f "$status_file" ]; then
        case "$(cat "$status_file")" in
            timeout) note=" (timed out after ${TEST_TIMEOUT}s)" ;;
            *) note=" (crashed: $(cat "$status_file"))" ;;
        esac
        rm -f "$status_file"
    fi
    memory_error="$(memory_error_kind asan_report.tmp)"
    # Keep the test's own stderr, minus the sanitizer report
    test_err="$(sed -e '/^=================================================================$/,$d' \
        -e '/^AddressSanitizer:DEADLYSIGNAL$/d' asan_report.tmp)"
    [ -n "$test_err" ] && output="${output:+$output
}$test_err"
    if [ -n "$memory_error" ]; then
        [ $part_is_bonus -eq 0 ] && break_score=1
        score_false=1
        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${name} ${RED}Memory fail (%s)${DEFAULT}\n" "$memory_error"
        [ -n "$output" ] && printf '%s\n' "$output"
        printf "    ${RED}Memory fail: %s${DEFAULT}\n" "$(memory_error_detail asan_report.tmp)"
    elif [ $code -eq 0 ]; then
        passed=$((passed+1))
        [ $selected_mode -eq 1 ] && selected_passed=$((selected_passed+1))
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
    rm -f asan_report.tmp
}

# Functions requested on the command line (./test.sh libft ft_strlen split).
# Empty means the whole suite. Names are normalized to the ft_ prefix.
selected=()
selected_mode=0
selected_passed=0
selected_total=0

is_selected()
{
    local f
    [ $selected_mode -eq 0 ] && return 0
    for f in "${selected[@]}"; do
        [ "$f" = "$1" ] && return 0
    done
    return 1
}

# Fills selected[] from the arguments after the suite name; exits with the
# list of valid names if one has no test in the suite.
select_functions()
{
    local suite_dir=$1 name bad=0
    shift
    [ $# -eq 0 ] && return
    selected_mode=1
    for name in "$@"; do
        case "$name" in
            ft_*) ;;
            *) name="ft_$name" ;;
        esac
        if ls "$suite_dir"/*/"$name.c" > /dev/null 2>&1; then
            selected+=("$name")
        else
            printf "${RED}No test for '%s' in %s.${DEFAULT}\n" "$name" "$(basename "$suite_dir")"
            bad=1
        fi
    done
    if [ $bad -eq 1 ]; then
        printf "Available functions:\n"
        for name in "$suite_dir"/*/*.c; do
            basename "${name%.c}"
        done | sort | tr '\n' ' '
        printf "\n"
        exit 1
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
            select_functions "$dir" "${@:2}"
            print_header
            printf "${GREEN} Generating test for ${1}...\n${DEFAULT}"
            space
            dirname_found=1
            index=0
            build_student_objects "$dir"

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
                test_files="$(collect_tests "$assignment")"
                if [ $selected_mode -eq 1 ]; then
                    # Only the requested functions run; setup scripts and
                    # parts without a requested function are skipped.
                    part_tests=""
                    for test in $test_files; do
                        case "$test" in
                            *.c) is_selected "$(basename "${test%.c}")" && part_tests+="$test " ;;
                        esac
                    done
                    [ -z "$part_tests" ] && continue
                    test_files="$part_tests"
                fi
                score_false=0
                part_is_bonus=0
                if [ "$assignment_name" = "bonus" ]; then
                    part_is_bonus=1
                    has_bonus=1
                else
                    questions=$((questions+1))
                fi
                printf "${PURPLE}${BOLD} ${assignment_name}${DEFAULT}\n"

                for test in $test_files; do
                    checks=$((checks+1))
                    [ $selected_mode -eq 1 ] && selected_total=$((selected_total+1))

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
                    fail_detail="$(student_compile_error "$fn_name")"
                    if [ -n "$fail_detail" ]; then
                        fail_reason="your ${fn_name}.c cannot compile"
                    elif ! student_src_exists "$fn_name"; then
                        fail_reason="no ${fn_name}.c found in your project"
                    fi

                    if [ -n "$fail_reason" ]; then
                        [ $part_is_bonus -eq 0 ] && break_score=1
                        score_false=1
                        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name} ${RED}(${fail_reason})${DEFAULT}\n"
                        [ -n "$fail_detail" ] && sed 's/^/    /' "$fail_detail" | head -15
                    elif cc $CC_FLAGS -o "${test%.c}" "$test" "$SCRIPT_DIR/utils/unbuffered_stdout.c" "${student_objs[@]}" 2> compile_error.tmp; then
                        run_test "$fn_name" "./${test%.c}"
                        rm -f "${test%.c}"
                    else
                        [ $part_is_bonus -eq 0 ] && break_score=1
                        score_false=1
                        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name} ${RED}(cannot compile)${DEFAULT}\n"
                        sed 's/^/    /' compile_error.tmp | head -15
                    fi
                    rm -f compile_error.tmp asan_report.tmp
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
    if [ $selected_mode -eq 1 ]; then
        print_selected_footer
    else
        print_footer
    fi
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
    printf "${BLUE}AddressSanitizer ${DEFAULT}on.\n"
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

# Footer for a run limited to some functions: no score, since the setup
# checks and the rest of the suite didn't run.
print_selected_footer()
{
    printf "${PURPLE}-----------------------------------${DEFAULT}\n"
    space
    if [ $selected_passed -eq $selected_total ]; then
        printf "Result:        ${GREEN}${selected_passed}/${selected_total} functions passed${DEFAULT}\n"
    else
        printf "Result:        ${RED}${selected_passed}/${selected_total} functions passed${DEFAULT}\n"
    fi
    printf "${GREY}Run mini with no function names for the full graded suite.${DEFAULT}\n"
    space
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
    printf "Please select a project. e.g. './test.sh libft' or './test.sh libft ft_strlen'\n"
    exit 1
fi
main "$@"
printf "$DEFAULT"
exit
