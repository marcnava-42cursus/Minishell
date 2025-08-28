#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    compare_with_bash.sh                               :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcos <marcos@student.42madrid.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/28 15:00:00 by marcos            #+#    #+#              #
#    Updated: 2025/08/28 15:00:00 by marcos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Script to compare minishell behavior with bash

set -e

# Configuration
MINISHELL="./minishell"
BASH="/bin/bash"
TEST_DIR="/tmp/minishell_bash_compare_$$"
TIMEOUT_DURATION="5s"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m'

# Test statistics
TOTAL_COMPARISONS=0
MATCHING_COMPARISONS=0
DIFFERENT_COMPARISONS=0

# Setup test environment
setup_test_env() {
    echo -e "${BLUE}Setting up comparison test environment...${NC}"
    mkdir -p "$TEST_DIR"
    
    # Check if minishell exists
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}ERROR: Minishell binary not found at $MINISHELL${NC}"
        echo "Run 'make' first to compile minishell"
        exit 1
    fi
    
    echo -e "${GREEN}Test environment ready${NC}"
    echo "Test directory: $TEST_DIR"
    echo ""
}

# Cleanup function
cleanup() {
    echo -e "\n${BLUE}Cleaning up test environment...${NC}"
    rm -rf "$TEST_DIR"
    
    # Kill any remaining processes
    pkill -f "$MINISHELL" 2>/dev/null || true
}

# Trap cleanup on exit
trap cleanup EXIT

# Execute command in both shells and compare
compare_shells() {
    local test_name="$1"
    local command="$2"
    local ignore_exit_code="${3:-false}"
    
    TOTAL_COMPARISONS=$((TOTAL_COMPARISONS + 1))
    
    echo -e "${YELLOW}Comparing: $test_name${NC}"
    echo "Command: $command"
    
    # Files for outputs
    local minishell_out="$TEST_DIR/minishell_out_$$"
    local minishell_err="$TEST_DIR/minishell_err_$$"
    local bash_out="$TEST_DIR/bash_out_$$"
    local bash_err="$TEST_DIR/bash_err_$$"
    
    # Run minishell
    local minishell_exit_code=0
    echo -e "$command\nexit" | timeout $TIMEOUT_DURATION "$MINISHELL" > "$minishell_out" 2> "$minishell_err" || minishell_exit_code=$?
    
    # Run bash in non-interactive mode
    local bash_exit_code=0
    echo -e "$command\nexit" | timeout $TIMEOUT_DURATION "$BASH" --norc --noprofile > "$bash_out" 2> "$bash_err" || bash_exit_code=$?
    
    # Read outputs
    local minishell_stdout=$(cat "$minishell_out" 2>/dev/null | grep -v "Minishell v" | grep -v "^>" || true)
    local minishell_stderr=$(cat "$minishell_err" 2>/dev/null || true)
    local bash_stdout=$(cat "$bash_out" 2>/dev/null || true)
    local bash_stderr=$(cat "$bash_err" 2>/dev/null || true)
    
    # Compare outputs and exit codes
    local outputs_match=true
    local exit_codes_match=true
    
    # Compare stdout
    if [ "$minishell_stdout" != "$bash_stdout" ]; then
        outputs_match=false
        echo -e "${RED}STDOUT DIFFERENT:${NC}"
        echo "Minishell: '$minishell_stdout'"
        echo "Bash:      '$bash_stdout'"
    fi
    
    # Compare stderr (simplified - just check if both have errors or not)
    if [ -n "$minishell_stderr" ] && [ -z "$bash_stderr" ]; then
        outputs_match=false
        echo -e "${RED}STDERR DIFFERENT: Minishell has error, bash doesn't${NC}"
        echo "Minishell stderr: $minishell_stderr"
    elif [ -z "$minishell_stderr" ] && [ -n "$bash_stderr" ]; then
        outputs_match=false
        echo -e "${RED}STDERR DIFFERENT: Bash has error, minishell doesn't${NC}"
        echo "Bash stderr: $bash_stderr"
    fi
    
    # Compare exit codes (if not ignoring them)
    if [ "$ignore_exit_code" != "true" ]; then
        if [ "$minishell_exit_code" != "$bash_exit_code" ]; then
            exit_codes_match=false
            echo -e "${RED}EXIT CODE DIFFERENT:${NC}"
            echo "Minishell: $minishell_exit_code"
            echo "Bash:      $bash_exit_code"
        fi
    fi
    
    # Final verdict
    if $outputs_match && ($exit_codes_match || [ "$ignore_exit_code" = "true" ]); then
        echo -e "${GREEN}✓ MATCH${NC}: Behaviors are identical"
        MATCHING_COMPARISONS=$((MATCHING_COMPARISONS + 1))
    else
        echo -e "${RED}✗ DIFFERENT${NC}: Behaviors differ"
        DIFFERENT_COMPARISONS=$((DIFFERENT_COMPARISONS + 1))
    fi
    
    echo "---"
    
    # Clean up temp files
    rm -f "$minishell_out" "$minishell_err" "$bash_out" "$bash_err"
}

# Test categories
run_basic_comparisons() {
    echo -e "\n${BLUE}=== BASIC COMMAND COMPARISONS ===${NC}\n"
    
    compare_shells "Simple echo" "echo hello world"
    compare_shells "Echo with quotes" "echo 'hello world'"
    compare_shells "Echo with double quotes" 'echo "hello world"'
    compare_shells "Echo with variable" 'export TEST=hello; echo $TEST'
    compare_shells "PWD command" "pwd"
    compare_shells "Environment variable" 'echo $HOME'
    compare_shells "Exit status" 'echo test; echo $?'
}

run_builtin_comparisons() {
    echo -e "\n${BLUE}=== BUILTIN COMMAND COMPARISONS ===${NC}\n"
    
    # CD tests (create temp dir first)
    local temp_test_dir="$TEST_DIR/cd_test"
    mkdir -p "$temp_test_dir"
    
    compare_shells "CD to directory" "cd $temp_test_dir; pwd"
    compare_shells "CD to home" "cd; pwd" true  # Ignore exit code due to different home handling
    compare_shells "Export variable" 'export TESTVAR=123; echo $TESTVAR'
    compare_shells "Unset variable" 'export TESTVAR=123; unset TESTVAR; echo $TESTVAR'
}

run_pipeline_comparisons() {
    echo -e "\n${BLUE}=== PIPELINE COMPARISONS ===${NC}\n"
    
    compare_shells "Simple pipe" "echo hello | cat"
    compare_shells "Three stage pipe" "echo test | cat | cat"
    compare_shells "Pipe with grep" "echo -e 'line1\\nline2\\ntest' | grep test"
    compare_shells "Multiple pipes" "echo hello world | grep hello | cat"
}

run_redirection_comparisons() {
    echo -e "\n${BLUE}=== REDIRECTION COMPARISONS ===${NC}\n"
    
    local test_file="$TEST_DIR/redirect_test.txt"
    
    # Output redirection
    compare_shells "Output redirection" "echo hello > $test_file; cat $test_file"
    
    # Append redirection
    compare_shells "Append redirection" "echo line1 > $test_file; echo line2 >> $test_file; cat $test_file"
    
    # Input redirection (create test file first)
    echo "test content" > "$test_file"
    compare_shells "Input redirection" "cat < $test_file"
}

run_error_comparisons() {
    echo -e "\n${BLUE}=== ERROR HANDLING COMPARISONS ===${NC}\n"
    
    compare_shells "Command not found" "nonexistent_command_xyz" true
    compare_shells "Invalid directory" "cd /nonexistent_directory_xyz" true
    compare_shells "Permission denied" "cat /root/.ssh/id_rsa" true  # Should fail for both
}

run_variable_comparisons() {
    echo -e "\n${BLUE}=== VARIABLE EXPANSION COMPARISONS ===${NC}\n"
    
    compare_shells "Basic variable expansion" 'export VAR=hello; echo $VAR'
    compare_shells "Variable in quotes" 'export VAR=hello; echo "$VAR world"'
    compare_shells "Variable concatenation" 'export VAR=hello; echo ${VAR}world'
    compare_shells "Undefined variable" 'echo $UNDEFINED_VARIABLE_XYZ'
}

run_quoting_comparisons() {
    echo -e "\n${BLUE}=== QUOTING COMPARISONS ===${NC}\n"
    
    compare_shells "Single quotes" "echo 'hello world'"
    compare_shells "Double quotes" 'echo "hello world"'
    compare_shells "Mixed quotes" 'echo "hello '\''world'\''""'
    compare_shells "Special characters" 'echo "!@#$%^&*()"'
}

run_edge_case_comparisons() {
    echo -e "\n${BLUE}=== EDGE CASE COMPARISONS ===${NC}\n"
    
    compare_shells "Empty command" ""
    compare_shells "Whitespace only" "   "
    compare_shells "Multiple spaces" "echo    hello    world"
    compare_shells "Tab characters" $'echo\thello\tworld'
}

# Main execution function
main() {
    echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${WHITE}         MINISHELL vs BASH BEHAVIOR COMPARISON             ${NC}"
    echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
    
    setup_test_env
    
    # Record start time
    local start_time=$(date +%s)
    
    # Run all comparison suites
    run_basic_comparisons
    run_builtin_comparisons
    run_pipeline_comparisons
    run_redirection_comparisons
    run_error_comparisons
    run_variable_comparisons
    run_quoting_comparisons
    run_edge_case_comparisons
    
    # Calculate execution time
    local end_time=$(date +%s)
    local duration=$((end_time - start_time))
    
    # Print final summary
    echo -e "\n${WHITE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${WHITE}                    COMPARISON RESULTS                      ${NC}"
    echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
    
    echo -e "Total Comparisons:  ${WHITE}$TOTAL_COMPARISONS${NC}"
    echo -e "Matching:           ${GREEN}$MATCHING_COMPARISONS${NC}"
    echo -e "Different:          ${RED}$DIFFERENT_COMPARISONS${NC}"
    echo -e "Execution Time:     ${CYAN}${duration}s${NC}"
    echo -e "Compatibility:      ${WHITE}$(( MATCHING_COMPARISONS * 100 / TOTAL_COMPARISONS ))%${NC}"
    
    if [ $DIFFERENT_COMPARISONS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 PERFECT COMPATIBILITY! 🎉${NC}"
        echo -e "${GREEN}Your minishell behaves identically to bash!${NC}"
        exit 0
    elif [ $DIFFERENT_COMPARISONS -le $(( TOTAL_COMPARISONS / 10 )) ]; then
        echo -e "\n${YELLOW}⚠️  MINOR DIFFERENCES DETECTED ⚠️${NC}"
        echo -e "${YELLOW}Most behaviors match bash. Review the differences above.${NC}"
        exit 0
    else
        echo -e "\n${RED}❌ SIGNIFICANT DIFFERENCES DETECTED ❌${NC}"
        echo -e "${YELLOW}Several behaviors differ from bash. Consider reviewing your implementation.${NC}"
        exit 1
    fi
}

# Usage function
usage() {
    echo -e "${BLUE}Usage: $0 [OPTION]${NC}"
    echo ""
    echo "Options:"
    echo "  -h, --help    Show this help message"
    echo ""
    echo "This script compares minishell behavior with bash by running"
    echo "identical commands in both shells and comparing the results."
    echo ""
    echo "The comparison includes:"
    echo "  - Standard output"
    echo "  - Standard error"
    echo "  - Exit codes"
    echo ""
    echo "Examples:"
    echo "  $0            # Run all comparisons"
}

# Parse arguments
case "${1:-}" in
    -h|--help)
        usage
        exit 0
        ;;
    "")
        main
        ;;
    *)
        echo -e "${RED}Error: Unknown option '$1'${NC}"
        usage
        exit 1
        ;;
esac
