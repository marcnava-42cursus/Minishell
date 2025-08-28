#!/bin/bash

# Comprehensive test script for minishell 
# Tests all implemented bash features including recent pipeline and heredoc fixes

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Comprehensive Minishell Test Suite ===${NC}\n"

test_count=0
passed_count=0

run_test() {
    local test_name="$1"
    local input="$2"
    local expected_pattern="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Run the test with timeout
    output=$(echo -e "$input" | timeout 5s $MINISHELL 2>/dev/null)
    
    if [[ $output =~ $expected_pattern ]]; then
        echo -e "${GREEN}PASS${NC}: Output matches expected pattern"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}FAIL${NC}: Output doesn't match expected pattern"
        echo "Expected pattern: $expected_pattern"
        echo "Got output: $output"
    fi
    echo "---"
}

run_test_exact() {
    local test_name="$1" 
    local input="$2"
    local expected_exact="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Extract just the command output (remove prompt lines)
    output=$(echo -e "$input" | timeout 5s $MINISHELL 2>/dev/null | grep -v "Minishell v" | grep -v ">" | grep -v "^$" | tr -d '\n')
    
    if [[ "$output" == "$expected_exact" ]]; then
        echo -e "${GREEN}PASS${NC}: Output matches exactly"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}FAIL${NC}: Output doesn't match exactly"
        echo "Expected: '$expected_exact'"
        echo "Got: '$output'"
    fi
    echo "---"
}

run_test_with_stderr() {
    local test_name="$1"
    local input="$2"
    local expected_pattern="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Run the test and capture both stdout and stderr
    output=$(echo -e "$input" | timeout 5s $MINISHELL 2>&1)
    
    if [[ $output =~ $expected_pattern ]]; then
        echo -e "${GREEN}PASS${NC}: Output matches expected pattern"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}FAIL${NC}: Output doesn't match expected pattern"
        echo "Expected pattern: $expected_pattern"
        echo "Got output: $output"
    fi
    echo "---"
}

run_test_syntax_error() {
    local test_name="$1"
    local input="$2"
    local expected_error="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Run the test and capture stderr
    output=$(echo -e "$input" | timeout 5s $MINISHELL 2>&1)
    
    if [[ $output == *"$expected_error"* ]]; then
        echo -e "${GREEN}PASS${NC}: Syntax error detected correctly"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}FAIL${NC}: Expected syntax error not found"
        echo "Expected error: $expected_error"
        echo "Got output: $output"
    fi
    echo "---"
}

# ========== BASIC FUNCTIONALITY TESTS ==========
echo -e "\n${BLUE}--- Basic Functionality ---${NC}"

run_test "Basic PWD command" "pwd\nexit" "$PWD"
run_test "Echo without options" "echo hello world\nexit" "hello world"
run_test "Echo with -n option" "echo -n test\nexit" "test"
run_test "Environment variable expansion" "echo \$HOME\nexit" "$HOME"
run_test "Exit status variable" "echo test\necho \$?\nexit" "0"

# ========== BUILT-IN COMMANDS TESTS ==========
echo -e "\n${BLUE}--- Built-in Commands ---${NC}"

run_test "CD command" "cd /tmp\npwd\nexit" "/tmp"
run_test "Export command" "export TEST=123\necho \$TEST\nexit" "123"
run_test "Unset command" "export TEST=123\nunset TEST\necho \"Result:[\$TEST]\"\nexit" "Result:\[\]"
run_test "Env command" "env | grep HOME\nexit" "HOME="

# ========== EXIT COMMAND TESTS ==========
echo -e "\n${BLUE}--- Exit Command Functionality ---${NC}"

# Test that exit actually terminates (should not see "should not appear")
run_test_exact "Exit terminates shell" "echo hello\nexit\necho should not appear" "hello"
run_test_exact "Exit with numeric code" "echo test\nexit 42" "test"  
# These tests need stderr captured since exit error messages go to stderr
run_test_with_stderr "Exit with invalid argument" "exit abc\necho still running\nexit" "numeric argument required"
run_test_with_stderr "Exit with too many arguments" "exit 1 2\necho still running\nexit" "too many arguments"

# ========== PIPELINE TESTS ==========  
echo -e "\n${BLUE}--- Pipeline Functionality ---${NC}"

run_test "Basic two-stage pipeline" "echo hello | cat\nexit" "hello"
run_test "Three-stage pipeline" "echo test | cat | cat\nexit" "test"
run_test "Pipeline with grep" "echo test | cat | grep test\nexit" "test" 
run_test "Cat-cat-ls pipeline" "cat | cat | ls\nexit" "test_minishell.sh"
run_test "Four-stage pipeline" "echo hello world | grep hello | cat | cat\nexit" "hello world"
run_test "Pipeline with filtering" "echo -e 'line1\\nline2\\nline3' | grep line2\nexit" "line2"

# ========== HEREDOC TESTS ==========
echo -e "\n${BLUE}--- Heredoc Functionality ---${NC}"

# Basic heredoc
run_test_exact "Basic heredoc" "cat << EOF\nhello\nworld\nEOF\nexit" "helloworld"

# Heredoc with unquoted delimiter (should expand content)
run_test_exact "Heredoc unquoted delimiter with expansion" "cat << eof\$USER\nhello \$USER\neof\$USER\nexit" "hello ivmirand"

# Heredoc with quoted delimiter (should not expand content)  
run_test_exact "Heredoc quoted delimiter no expansion" "cat << \"eof\$USER\"\nhello \$USER\neof\$USER\nexit" "hello \$USER"

# Heredoc with mixed quoted delimiter (should not expand content)
run_test_exact "Heredoc mixed quoted delimiter" "cat << eof\"\$USER\"\nhello \$USER\neof\$USER\nexit" "hello \$USER"

# Complex heredoc delimiter concatenation
run_test_exact "Heredoc complex delimiter" "cat << \"end\"\$USER\nhello world\nend\$USER\nexit" "hello world"

# ========== REDIRECTION TESTS ==========
echo -e "\n${BLUE}--- Redirection Functionality ---${NC}"

run_test "Input redirection" "echo hello > /tmp/test.txt\ncat < /tmp/test.txt\nrm /tmp/test.txt\nexit" "hello"
run_test "Output redirection" "echo test > /tmp/output.txt\ncat /tmp/output.txt\nrm /tmp/output.txt\nexit" "test"  
run_test "Append redirection" "echo line1 > /tmp/append.txt\necho line2 >> /tmp/append.txt\ncat /tmp/append.txt\nrm /tmp/append.txt\nexit" "line1.*line2"

# ========== QUOTING TESTS ==========
echo -e "\n${BLUE}--- Quoting and Variable Expansion ---${NC}"

run_test "Single quoted strings" "echo 'hello world'\nexit" "hello world"
run_test "Double quoted strings with variables" "export VAR=test\necho \"Value: \$VAR\"\nexit" "Value: test"
run_test "Mixed quotes" "echo 'single' \"double\" unquoted\nexit" "single double unquoted"
run_test "Variable expansion in unquoted" "export TEST=hello\necho \$TEST world\nexit" "hello world"

# ========== SYNTAX ERROR TESTS ==========
echo -e "\n${BLUE}--- Syntax Error Detection ---${NC}"

run_test_syntax_error "Pipe at beginning" "| ls" "syntax error near unexpected token"
run_test_syntax_error "Input redirection + pipe" "cat <| ls" "syntax error near unexpected token"  
run_test_syntax_error "Output redirection + pipe" "cat >| ls" "syntax error near unexpected token"
run_test_syntax_error "Command ending with pipe" "echo hello |" "syntax error near unexpected"
run_test_syntax_error "Command ending with redirection" "echo hello >" "syntax error near unexpected"

# ========== EDGE CASES ==========
echo -e "\n${BLUE}--- Edge Cases ---${NC}"

run_test "Empty command" "\nexit" "Minishell v"
run_test "Whitespace handling" "   echo   hello   world   \nexit" "hello world"
run_test "Multiple environment variables" "export A=1 B=2\necho \$A \$B\nexit" "1 2"

# ========== TEST SUMMARY ==========
echo -e "\n${BLUE}=== Test Summary ===${NC}"
echo "Tests run: $test_count"
echo "Tests passed: $passed_count"  
echo "Tests failed: $((test_count - passed_count))"

if [ $passed_count -eq $test_count ]; then
    echo -e "${GREEN}🎉 All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}❌ Some tests failed.${NC}"
    echo -e "Success rate: $((passed_count * 100 / test_count))%"
    exit 1
fi
