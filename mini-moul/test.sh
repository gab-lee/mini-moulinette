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

main()
{
    start_time=$(date +%s)
    #print_collected_files
    for dir in ./tests/* ; do
        dirname="$(basename "$dir")"
        case "$dirname" in
            *"(archive)"*) continue ;;
        esac
        available_assignments+="$dirname "
        
        if [ -d "$dir" ] && [ "$dirname" == "$1" ]; then
            dirname_found=1
            print_header
            printf "${GREEN} Generating test for ${1}...\n${DEFAULT}"
            space
            dirname_found=1
            index=0
            build_student_objects

            # Run parts in subject order (setup, libc, additional, bonus),
            # then anything else
            exercise_dirs=""
            for part in setup libc additional bonus; do
                [ -d "$dir/$part" ] && exercise_dirs+="$dir/$part "
            done
            for part in $dir/*; do
                case "$(basename "$part")" in
                    setup|libc|additional|bonus) continue ;;
                esac
                exercise_dirs+="$part "
            done

            for assignment in $exercise_dirs; do
                [ -d "$assignment" ] || continue
                questions=$((questions+1))
                score_false=0
                assignment_name="$(basename "$assignment")"
                printf "${PURPLE}${BOLD} ${assignment_name}${DEFAULT}\n"
                test_files="$(collect_tests "$assignment")"

                for test in $test_files; do
                    checks=$((checks+1))

                    # Shell check scripts (setup part) run as-is, with the
                    # mini-moul directory as cwd and the project at ../
                    case "$test" in
                        *.sh)
                            fn_name="$(basename "${test%.sh}")"
                            test_output="$(bash "$test" 2>&1)"
                            if [ $? -eq 0 ]; then
                                passed=$((passed+1))
                                printf " ${BG_GREEN}${BLACK}${BOLD} PASS ${DEFAULT} ${fn_name}\n"
                            else
                                break_score=1
                                score_false=1
                                printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name}\n"
                                printf '%s\n' "$test_output"
                            fi
                            continue
                            ;;
                    esac

                    fn_name="$(basename "${test%.c}")"
                    src_err="$(student_compile_error "$fn_name")"

                    if [ -n "$src_err" ]; then
                        break_score=1
                        score_false=1
                        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name} ${RED}(your ${fn_name}.c cannot compile)${DEFAULT}\n"
                        sed 's/^/    /' "$src_err" | head -15
                    elif ! student_src_exists "$fn_name"; then
                        break_score=1
                        score_false=1
                        printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name} ${RED}(no ${fn_name}.c found in your project)${DEFAULT}\n"
                    elif cc -Wall -Werror -Wextra -o "${test%.c}" "$test" "${student_objs[@]}" 2> compile_error.tmp; then
                        test_output="$("./${test%.c}" 2>&1)"
                        if [ $? -eq 0 ]; then
                            passed=$((passed+1))
                            printf " ${BG_GREEN}${BLACK}${BOLD} PASS ${DEFAULT} ${fn_name}\n"
                            # Known-strict cases surface as [!] warnings even on PASS
                            case "$test_output" in
                                *"[!]"*) printf '%s\n' "$test_output" | grep -F '[!]' ;;
                            esac
                        else
                            break_score=1
                            score_false=1
                            printf " ${BG_RED}${BOLD} FAIL ${DEFAULT} ${fn_name}\n"
                            printf '%s\n' "$test_output"
                        fi
                        rm -f "${test%.c}"
                    else
                        break_score=1
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
    printf "${BLUE}Mini moulinette ${DEFAULT}version 0.0.1 8 July 2026.\n"
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
    if [ $break_score = 0 ]; then
        marks=$((marks+1))
    fi
}

print_footer()
{
    printf "${PURPLE}-----------------------------------${DEFAULT}\n"
    space
    PERCENT=$((100 * marks / questions))
    #printf "Total checks:  ""${GREEN}${passed} passed  ${DEFAULT} ""${checks} total"
    printf "Result:        ${result}\n"
    if [ $PERCENT -ge 50 ]; then
        printf "Final score:   ""${GREEN}$(echo $PERCENT | bc)/100${DEFAULT}\n"
        printf "Status:        ""${GREEN}passed${DEFAULT}\n"
    else
        printf "Final score:   ""${RED}$(echo $PERCENT | bc)/100${DEFAULT}\n"
        printf "Status:        ""${RED}FAILED${DEFAULT}\n"
    fi
    end_time=$(date +%s)
    elapsed_time=$(expr $end_time - $start_time)
    printf "${GREY}Test completed. ${PINK}Total elapsed time: ${elapsed_time}s${DEFAULT}.\n"
    printf "${BLUE}Mini moulinette is updated daily. Please remember to git pull today!\n${DEFAULT}"
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
