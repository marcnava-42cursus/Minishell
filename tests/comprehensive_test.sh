#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    comprehensive_test.sh                              :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcos <marcos@student.42madrid.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/28 15:00:00 by marcos            #+#    #+#              #
#    Updated: 2025/08/28 15:00:00 by marcos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Comprehensive minishell test that handles:
# - Exit codes
# - Error outputs (stdout/stderr)
# - Signal handling
# - Memory leaks (valgrind integration)
# - Performance tests
# - Edge cases and boundary conditions

set -e

# Configuration
MINISHELL="./minishell"
TIMEOUT_CMD="timeout"
TIMEOUT_DURATION="10s"
VALGRIND_CMD="valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --quiet"
TEST_DIR="/tmp/minishell_test_$$"
LOG_FILE="$TEST_DIR/test_results.log"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m' # No Color

# Test statistics
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
SKIPPED_TESTS=0

# Create test environment
setup_test_env() {
    echo -e "${BLUE}Setting up test environment...${NC}"
    mkdir -p "$TEST_DIR"
    touch "$LOG_FILE"
    
    # Check if minishell exists
    if [ ! -f "$MINISHELL" ]; then
        echo -e "${RED}ERROR: Minishell binary not found at $MINISHELL${NC}"
        echo "Run 'make' first to compile minishell"
        exit 1
    fi
    
    # Check if timeout command exists
    if ! command -v timeout &> /dev/null; then
        TIMEOUT_CMD="gtimeout"  # macOS alternative
        if ! command -v gtimeout &> /dev/null; then
            echo -e "${YELLOW}WARNING: timeout command not found, tests may hang${NC}"
            TIMEOUT_CMD=""
        fi
    fi
    
    # Check if valgrind is available
    if ! command -v valgrind &> /dev/null; then
        echo -e "${YELLOW}WARNING: valgrind not found, memory leak tests will be skipped${NC}"
        VALGRIND_CMD=""
    fi
    
    echo -e "${GREEN}Test environment ready${NC}"
    echo "Test directory: $TEST_DIR"
    echo "Log file: $LOG_FILE"
    echo ""
}

# Cleanup function
cleanup() {
    echo -e "\n${BLUE}Cleaning up test environment...${NC}"
    rm -rf "$TEST_DIR"
    
    # Kill any remaining minishell processes
    pkill -f "$MINISHELL" 2>/dev/null || true
}

# Trap cleanup on exit
trap cleanup EXIT

# Logging function
log_test() {
    local timestamp=$(date '+%Y-%m-%d %H:%M:%S')
    echo "[$timestamp] $1" >> "$LOG_FILE"
}

# Test result tracking
track_result() {
    local result=$1
    TOTAL_TESTS=$((TOTAL_TESTS + 1))
    
    case $result in
        "PASS") PASSED_TESTS=$((PASSED_TESTS + 1)) ;;
        "FAIL") FAILED_TESTS=$((FAILED_TESTS + 1)) ;;
        "SKIP") SKIPPED_TESTS=$((SKIPPED_TESTS + 1)) ;;
    esac
}

# Execute command with timeout and capture all output
run_command() {
    local input="$1"
    local timeout_val="${2:-$TIMEOUT_DURATION}"
    local temp_file="$TEST_DIR/cmd_output_$$"
    local stderr_file="$TEST_DIR/cmd_stderr_$$"
    local exit_code_file="$TEST_DIR/cmd_exit_$$"
    
    # Clear previous outputs
    > "$temp_file"
    > "$stderr_file"
    > "$exit_code_file"
    
    # Run command with timeout
    if [ -n "$TIMEOUT_CMD" ]; then
        echo -e "$input" | $TIMEOUT_CMD "$timeout_val" "$MINISHELL" > "$temp_file" 2> "$stderr_file"
        echo $? > "$exit_code_file"
    else
        echo -e "$input" | "$MINISHELL" > "$temp_file" 2> "$stderr_file" &
        local pid=$!
        sleep 3
        kill $pid 2>/dev/null || true
        wait $pid 2>/dev/null
        echo $? > "$exit_code_file"
    fi
    
    # Return outputs via global variables
    CMD_STDOUT=$(cat "$temp_file" 2>/dev/null || echo "")
    CMD_STDERR=$(cat "$stderr_file" 2>/dev/null || echo "")
    CMD_EXIT_CODE=$(cat "$exit_code_file" 2>/dev/null || echo "1")
    
    # Clean up temp files
    rm -f "$temp_file" "$stderr_file" "$exit_code_file"
}

# Basic test function
test_basic() {
    local test_name="$1"
    local input="$2"
    local expected_stdout="$3"
    local expected_stderr="${4:-}"
    local expected_exit_code="${5:-0}"
    
    echo -e "${YELLOW}Testing: $test_name${NC}"
    log_test "TESTING: $test_name"
    
    run_command "$input"
    
    local success=true
    local error_msg=""
    
    # Check stdout
    if [ -n "$expected_stdout" ] && [[ ! "$CMD_STDOUT" =~ $expected_stdout ]]; then
        success=false
        error_msg+="STDOUT mismatch. Expected: '$expected_stdout', Got: '$CMD_STDOUT'\n"
    fi
    
    # Check stderr
    if [ -n "$expected_stderr" ] && [[ ! "$CMD_STDERR" =~ $expected_stderr ]]; then
        success=false
        error_msg+="STDERR mismatch. Expected: '$expected_stderr', Got: '$CMD_STDERR'\n"
    fi
    
    # Check exit code
    if [ "$CMD_EXIT_CODE" != "$expected_exit_code" ]; then
        success=false
        error_msg+="EXIT_CODE mismatch. Expected: $expected_exit_code, Got: $CMD_EXIT_CODE\n"
    fi
    
    if $success; then
        echo -e "${GREEN}✓ PASS${NC}: $test_name"
        log_test "PASS: $test_name"
        track_result "PASS"
    else
        echo -e "${RED}✗ FAIL${NC}: $test_name"
        echo -e "${RED}$error_msg${NC}"
        log_test "FAIL: $test_name - $error_msg"
        track_result "FAIL"
    fi
    echo "---"
}

# Memory leak test
test_memory_leak() {
    local test_name="$1"
    local input="$2"
    
    if [ -z "$VALGRIND_CMD" ]; then
        echo -e "${YELLOW}SKIP${NC}: $test_name (valgrind not available)"
        track_result "SKIP"
        return
    fi
    
    echo -e "${PURPLE}Memory Test: $test_name${NC}"
    log_test "MEMORY_TEST: $test_name"
    
    local valgrind_output="$TEST_DIR/valgrind_output_$$"
    echo -e "$input" | $VALGRIND_CMD "$MINISHELL" 2> "$valgrind_output" >/dev/null
    
    if grep -q "no leaks are possible" "$valgrind_output" || grep -q "All heap blocks were freed" "$valgrind_output"; then
        echo -e "${GREEN}✓ PASS${NC}: No memory leaks detected"
        log_test "MEMORY_PASS: $test_name"
        track_result "PASS"
    else
        echo -e "${RED}✗ FAIL${NC}: Memory leaks detected"
        echo "Valgrind output:"
        cat "$valgrind_output" | head -20
        log_test "MEMORY_FAIL: $test_name"
        track_result "FAIL"
    fi
    
    rm -f "$valgrind_output"
    echo "---"
}

# Signal handling test
test_signal_handling() {
    local test_name="$1"
    local signal="$2"
    
    echo -e "${CYAN}Signal Test: $test_name${NC}"
    log_test "SIGNAL_TEST: $test_name"
    
    # Start minishell in background
    "$MINISHELL" > "$TEST_DIR/signal_out" 2> "$TEST_DIR/signal_err" &
    local pid=$!
    
    sleep 1
    
    # Send signal
    kill -$signal $pid 2>/dev/null || true
    
    sleep 1
    
    # Check if process is still running (for SIGINT, SIGQUIT)
    if kill -0 $pid 2>/dev/null; then
        if [ "$signal" = "TERM" ] || [ "$signal" = "KILL" ]; then
            echo -e "${RED}✗ FAIL${NC}: Process should have terminated"
            track_result "FAIL"
        else
            echo -e "${GREEN}✓ PASS${NC}: Process correctly handled signal"
            track_result "PASS"
        fi
        kill -KILL $pid 2>/dev/null || true
    else
        if [ "$signal" = "TERM" ] || [ "$signal" = "KILL" ]; then
            echo -e "${GREEN}✓ PASS${NC}: Process correctly terminated"
            track_result "PASS"
        else
            echo -e "${RED}✗ FAIL${NC}: Process unexpectedly terminated"
            track_result "FAIL"
        fi
    fi
    
    wait $pid 2>/dev/null || true
    echo "---"
}

# Performance test
test_performance() {
    local test_name="$1"
    local input="$2"
    local max_time="$3"
    
    echo -e "${WHITE}Performance Test: $test_name${NC}"
    log_test "PERFORMANCE_TEST: $test_name"
    
    local start_time=$(date +%s.%N)
    run_command "$input" "${max_time}s"
    local end_time=$(date +%s.%N)
    
    local duration=$(echo "$end_time - $start_time" | bc 2>/dev/null || echo "0")
    
    if [ "$CMD_EXIT_CODE" = "124" ]; then  # timeout exit code
        echo -e "${RED}✗ FAIL${NC}: Test timed out after ${max_time}s"
        track_result "FAIL"
    elif (( $(echo "$duration > $max_time" | bc -l 2>/dev/null || echo "0") )); then
        echo -e "${RED}✗ FAIL${NC}: Test took ${duration}s (max: ${max_time}s)"
        track_result "FAIL"
    else
        echo -e "${GREEN}✓ PASS${NC}: Test completed in ${duration}s"
        track_result "PASS"
    fi
    
    log_test "PERFORMANCE: $test_name - Duration: ${duration}s"
    echo "---"
}

# File system test
test_filesystem() {
    local test_name="$1"
    local input="$2"
    local expected_file="$3"
    local expected_content="$4"
    
    echo -e "${CYAN}Filesystem Test: $test_name${NC}"
    log_test "FILESYSTEM_TEST: $test_name"
    
    # Clean up any existing test file
    rm -f "$expected_file" 2>/dev/null || true
    
    run_command "$input"
    
    if [ -f "$expected_file" ]; then
        if [ -n "$expected_content" ]; then
            local actual_content=$(cat "$expected_file" 2>/dev/null || echo "")
            if [ "$actual_content" = "$expected_content" ]; then
                echo -e "${GREEN}✓ PASS${NC}: File created with correct content"
                track_result "PASS"
            else
                echo -e "${RED}✗ FAIL${NC}: File content mismatch"
                echo "Expected: '$expected_content'"
                echo "Got: '$actual_content'"
                track_result "FAIL"
            fi
        else
            echo -e "${GREEN}✓ PASS${NC}: File created successfully"
            track_result "PASS"
        fi
    else
        echo -e "${RED}✗ FAIL${NC}: Expected file not created: $expected_file"
        track_result "FAIL"
    fi
    
    # Clean up
    rm -f "$expected_file" 2>/dev/null || true
    echo "---"
}

# ============================================================================
# TEST SUITES
# ============================================================================

run_basic_tests() {
    echo -e "\n${BLUE}=== BASIC FUNCTIONALITY TESTS ===${NC}\n"
    
    test_basic "Simple echo" "echo hello\nexit" "hello"
    test_basic "Echo with quotes" "echo 'hello world'\nexit" "hello world"
    test_basic "Echo with variables" "export TEST=world\necho hello \$TEST\nexit" "hello world"
    test_basic "PWD command" "pwd\nexit" "$PWD"
    test_basic "Empty input handling" "\n\n\nexit" ""
    test_basic "Whitespace handling" "   echo   test   \nexit" "test"
}

run_builtin_tests() {
    echo -e "\n${BLUE}=== BUILTIN COMMANDS TESTS ===${NC}\n"
    
    test_basic "CD to /tmp" "cd /tmp\npwd\nexit" "/tmp"
    test_basic "CD to home" "cd\npwd\nexit" "$HOME"
    test_basic "CD to nonexistent" "cd /nonexistent\nexit" "" "No such file" 1
    
    test_basic "Export new variable" "export TESTVAR=123\necho \$TESTVAR\nexit" "123"
    test_basic "Export multiple variables" "export A=1 B=2\necho \$A \$B\nexit" "1 2"
    
    test_basic "Unset variable" "export TESTVAR=123\nunset TESTVAR\necho \$TESTVAR\nexit" ""
    
    test_basic "Env command" "env | grep HOME\nexit" "HOME="
    
    test_basic "Exit with code" "exit 42" "" "" 42
    test_basic "Exit with invalid argument" "exit abc\nexit" "" "numeric argument required"
}

run_pipeline_tests() {
    echo -e "\n${BLUE}=== PIPELINE TESTS ===${NC}\n"
    
    test_basic "Simple pipe" "echo hello | cat\nexit" "hello"
    test_basic "Three-stage pipe" "echo test | cat | cat\nexit" "test"
    test_basic "Pipe with grep" "echo -e 'line1\\nline2\\ntest' | grep test\nexit" "test"
    test_basic "Multiple pipes" "echo hello world | grep hello | cat | cat\nexit" "hello world"
    
    # Error cases
    test_basic "Pipe at start" "| echo test\nexit" "" "syntax error" 2
    test_basic "Pipe at end" "echo test |\nexit" "" "syntax error" 2
}

run_redirection_tests() {
    echo -e "\n${BLUE}=== REDIRECTION TESTS ===${NC}\n"
    
    local test_file="$TEST_DIR/redirect_test.txt"
    
    test_filesystem "Output redirection" "echo hello > $test_file\nexit" "$test_file" "hello"
    test_filesystem "Append redirection" "echo line1 > $test_file\necho line2 >> $test_file\nexit" "$test_file" "line1\nline2"
    
    # Create input file for input redirection test
    echo "input_content" > "$test_file"
    test_basic "Input redirection" "cat < $test_file\nexit" "input_content"
    
    # Error cases
    test_basic "Redirect to directory" "echo test > /tmp\nexit" "" "Is a directory" 1
    test_basic "Redirect from nonexistent" "cat < /nonexistent\nexit" "" "No such file" 1
}

run_heredoc_tests() {
    echo -e "\n${BLUE}=== HEREDOC TESTS ===${NC}\n"
    
    test_basic "Basic heredoc" "cat << EOF\nhello\nworld\nEOF\nexit" "hello\nworld"
    test_basic "Heredoc with variables" "export USER=test\ncat << EOF\nhello \$USER\nEOF\nexit" "hello test"
    test_basic "Quoted heredoc delimiter" "cat << 'EOF'\nhello \$USER\nEOF\nexit" "hello \\\$USER"
}

run_error_handling_tests() {
    echo -e "\n${BLUE}=== ERROR HANDLING TESTS ===${NC}\n"
    
    test_basic "Command not found" "nonexistent_command\nexit" "" "command not found" 127
    test_basic "Invalid syntax" "echo >\nexit" "" "syntax error" 2
    test_basic "Unclosed quotes" "echo 'unclosed\nexit" "" "unexpected EOF" 2
    
    # Permission errors
    test_basic "Permission denied" "/root/.profile\nexit" "" "Permission denied" 126
}

run_signal_tests() {
    echo -e "\n${BLUE}=== SIGNAL HANDLING TESTS ===${NC}\n"
    
    test_signal_handling "SIGINT handling" "INT"
    test_signal_handling "SIGQUIT handling" "QUIT"
    test_signal_handling "SIGTERM handling" "TERM"
}

run_memory_tests() {
    echo -e "\n${BLUE}=== MEMORY LEAK TESTS ===${NC}\n"
    
    test_memory_leak "Basic commands" "echo test\npwd\nenv\nexit"
    test_memory_leak "Complex pipeline" "echo hello | grep hello | cat\nexit"
    test_memory_leak "Variable expansion" "export TEST=hello\necho \$TEST world\nexit"
    test_memory_leak "Multiple commands" "pwd\necho test\ncd /tmp\npwd\nexit"
}

run_performance_tests() {
    echo -e "\n${BLUE}=== PERFORMANCE TESTS ===${NC}\n"
    
    test_performance "Quick command" "echo test\nexit" 2
    test_performance "Multiple commands" "$(for i in {1..50}; do echo "echo test$i"; done)\nexit" 5
    test_performance "Complex pipeline" "echo test | cat | cat | cat | cat | cat\nexit" 3
}

run_edge_case_tests() {
    echo -e "\n${BLUE}=== EDGE CASE TESTS ===${NC}\n"
    
    test_basic "Very long command line" "echo $(printf 'a%.0s' {1..1000})\nexit" "$(printf 'a%.0s' {1..1000})"
    test_basic "Many arguments" "echo $(for i in {1..100}; do echo -n "arg$i "; done)\nexit" "$(for i in {1..100}; do echo -n "arg$i "; done)"
    test_basic "Special characters" "echo '!@#\$%^&*()'\nexit" "!@#\\\$%^&*()"
    test_basic "Unicode characters" "echo 'héllo wörld 🌍'\nexit" "héllo wörld 🌍"
    
    # Boundary conditions
    test_basic "Empty string variable" "export EMPTY=''\necho \"[\$EMPTY]\"\nexit" "\\[\\]"
    test_basic "Variable with spaces" "export SPACED='hello world'\necho \$SPACED\nexit" "hello world"
}

run_integration_tests() {
    echo -e "\n${BLUE}=== INTEGRATION TESTS ===${NC}\n"
    
    # Complex scenarios combining multiple features
    test_basic "Complex scenario 1" "export PATH=/bin:/usr/bin\necho hello | grep hello > $TEST_DIR/out.txt\ncat < $TEST_DIR/out.txt\nexit" "hello"
    
    test_basic "Complex scenario 2" "cd /tmp\npwd\necho \$PWD\ncd -\npwd\nexit" "/tmp\n/tmp\n$PWD"
    
    # Nested operations
    test_basic "Nested operations" "echo \$(echo inner)\nexit" "inner" "" 0
}

# ============================================================================
# MAIN EXECUTION
# ============================================================================

main() {
    echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${WHITE}    COMPREHENSIVE MINISHELL TEST SUITE                     ${NC}"
    echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
    
    setup_test_env
    
    # Record start time
    local start_time=$(date +%s)
    
    # Run all test suites
    run_basic_tests
    run_builtin_tests
    run_pipeline_tests
    run_redirection_tests
    run_heredoc_tests
    run_error_handling_tests
    run_signal_tests
    run_memory_tests
    run_performance_tests
    run_edge_case_tests
    run_integration_tests
    
    # Calculate execution time
    local end_time=$(date +%s)
    local duration=$((end_time - start_time))
    
    # Print final summary
    echo -e "\n${WHITE}═══════════════════════════════════════════════════════════${NC}"
    echo -e "${WHITE}                        FINAL RESULTS                       ${NC}"
    echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
    
    echo -e "Total Tests Run:    ${WHITE}$TOTAL_TESTS${NC}"
    echo -e "Tests Passed:       ${GREEN}$PASSED_TESTS${NC}"
    echo -e "Tests Failed:       ${RED}$FAILED_TESTS${NC}"
    echo -e "Tests Skipped:      ${YELLOW}$SKIPPED_TESTS${NC}"
    echo -e "Execution Time:     ${CYAN}${duration}s${NC}"
    echo -e "Success Rate:       ${WHITE}$(( PASSED_TESTS * 100 / TOTAL_TESTS ))%${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}🎉 ALL TESTS PASSED! 🎉${NC}"
        echo -e "${GREEN}Your minishell implementation is working correctly!${NC}"
        exit 0
    else
        echo -e "\n${RED}❌ SOME TESTS FAILED ❌${NC}"
        echo -e "${YELLOW}Check the detailed log at: $LOG_FILE${NC}"
        echo -e "${YELLOW}Review failed tests and fix the implementation${NC}"
        exit 1
    fi
}

# Check if script is being sourced or executed
if [[ "${BASH_SOURCE[0]}" == "${0}" ]]; then
    main "$@"
fi
