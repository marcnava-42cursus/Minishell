#!/bin/bash

# Advanced Minishell Stress Test Suite
# Tests complex bash commands and edge cases to identify missing features or bugs

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

echo -e "${PURPLE}=== Advanced Minishell Stress Test Suite ===${NC}\n"
echo -e "${BLUE}Testing complex bash commands and edge cases${NC}\n"

test_count=0
passed_count=0
expected_failures=0

# Function to run a test and compare with bash behavior
run_stress_test() {
    local test_name="$1"
    local input="$2"
    local should_match_bash="$3"  # true/false - whether minishell should match bash exactly
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Get bash output (reference)
    bash_output=$(echo -e "$input" | timeout 5s bash 2>&1 || true)
    bash_exit=$?
    
    # Get minishell output
    minishell_output=$(echo -e "$input" | timeout 5s $MINISHELL 2>&1 || true)
    minishell_exit=$?
    
    if [[ "$should_match_bash" == "true" ]]; then
        # Strict comparison - should match bash exactly
        if [[ "$minishell_output" == "$bash_output" ]] && [[ "$minishell_exit" == "$bash_exit" ]]; then
            echo -e "${GREEN}PASS${NC}: Matches bash behavior exactly"
            passed_count=$((passed_count + 1))
        else
            echo -e "${RED}FAIL${NC}: Behavior differs from bash"
            echo "Bash output: '$bash_output' (exit: $bash_exit)"
            echo "Minishell output: '$minishell_output' (exit: $minishell_exit)"
        fi
    else
        # Expected difference - just document the behavior
        echo -e "${BLUE}INFO${NC}: Expected difference from bash (feature not implemented)"
        echo "Bash: '$bash_output'"
        echo "Minishell: '$minishell_output'"
        expected_failures=$((expected_failures + 1))
    fi
    echo "---"
}

# Function for tests that should work but might have minor formatting differences
run_functional_test() {
    local test_name="$1"
    local input="$2"
    local expected_pattern="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    minishell_output=$(echo -e "$input" | timeout 5s $MINISHELL 2>&1 || true)
    
    if [[ $minishell_output =~ $expected_pattern ]]; then
        echo -e "${GREEN}PASS${NC}: Expected functionality works"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}FAIL${NC}: Expected functionality missing"
        echo "Expected pattern: $expected_pattern"
        echo "Got: '$minishell_output'"
    fi
    echo "---"
}

# ========== COMPLEX PIPELINE TESTS ==========
echo -e "\n${BLUE}--- Complex Pipeline Tests ---${NC}"

run_stress_test "Long pipeline chain" \
    "echo 'line1\nline2\nline3\nline4' | grep line | head -2 | tail -1 | cat\nexit" \
    "true"

run_stress_test "Pipeline with multiple greps" \
    "echo 'apple banana cherry date' | grep a | grep e\nexit" \
    "true"

run_stress_test "Pipeline with built-ins and external commands" \
    "env | head -5 | grep -E '^[A-Z]' | wc -l\nexit" \
    "false"  # wc not implemented

run_stress_test "Pipeline ending with built-in" \
    "echo 'PATH=/bin:/usr/bin' | env\nexit" \
    "false"  # env with args not standard

# ========== COMPLEX REDIRECTION TESTS ==========
echo -e "\n${BLUE}--- Complex Redirection Tests ---${NC}"

run_stress_test "Multiple redirections" \
    "echo hello > /tmp/test1.txt > /tmp/test2.txt\ncat /tmp/test2.txt\nrm /tmp/test1.txt /tmp/test2.txt\nexit" \
    "true"

run_stress_test "Input and output redirection combined" \
    "echo 'test content' > /tmp/input.txt\ncat < /tmp/input.txt > /tmp/output.txt\ncat /tmp/output.txt\nrm /tmp/input.txt /tmp/output.txt\nexit" \
    "true"

run_stress_test "Redirection with pipeline" \
    "echo hello | cat > /tmp/pipe_out.txt\ncat /tmp/pipe_out.txt\nrm /tmp/pipe_out.txt\nexit" \
    "true"

# ========== COMPLEX HEREDOC TESTS ==========
echo -e "\n${BLUE}--- Complex Heredoc Tests ---${NC}"

run_stress_test "Nested variable expansion in heredoc" \
    "export OUTER=world\nexport INNER=OUTER\ncat << EOF\nhello \${\$INNER}\nEOF\nexit" \
    "false"  # Complex expansion not implemented

run_stress_test "Heredoc with pipeline" \
    "cat << EOF | grep hello\nhello world\ngoodbye world\nEOF\nexit" \
    "true"

run_stress_test "Multiple heredocs" \
    "cat << EOF1 << EOF2\nfrom first\nEOF1\nfrom second\nEOF2\nexit" \
    "false"  # Multiple heredocs edge case

# ========== VARIABLE EXPANSION EDGE CASES ==========
echo -e "\n${BLUE}--- Variable Expansion Edge Cases ---${NC}"

run_stress_test "Variable with default value" \
    "echo \${NONEXISTENT:-default}\nexit" \
    "false"  # Parameter expansion not implemented

run_stress_test "Nested variable expansion" \
    "export VAR1=VAR2\nexport VAR2=hello\necho \$\$VAR1\nexit" \
    "false"  # Complex expansion

run_stress_test "Variable expansion in quotes" \
    "export TEST='hello world'\necho \"\$TEST\" '\$TEST'\nexit" \
    "true"

run_stress_test "Exit status in complex expression" \
    "false\necho \"Exit was: \$?\"\ntrue\necho \"Exit now: \$?\"\nexit" \
    "true"

# ========== QUOTING EDGE CASES ==========
echo -e "\n${BLUE}--- Quoting Edge Cases ---${NC}"

run_stress_test "Escaped quotes in double quotes" \
    "echo \"He said \\\"hello\\\"\"\nexit" \
    "false"  # Escape sequences in quotes

run_stress_test "Mixed nested quotes" \
    "echo 'single \"double\" single'\nexit" \
    "true"

run_stress_test "Empty quoted strings" \
    "echo '' \"\" ''\nexit" \
    "true"

run_stress_test "Quotes with redirections" \
    "echo 'hello world' > '/tmp/quoted file.txt'\ncat < '/tmp/quoted file.txt'\nrm '/tmp/quoted file.txt'\nexit" \
    "true"

# ========== BUILT-IN COMMAND EDGE CASES ==========
echo -e "\n${BLUE}--- Built-in Command Edge Cases ---${NC}"

run_stress_test "CD with tilde expansion" \
    "cd ~\npwd\nexit" \
    "false"  # Tilde expansion not implemented

run_stress_test "Export with complex assignment" \
    "export VAR=\"hello world\" VAR2=test\necho \$VAR \$VAR2\nexit" \
    "true"

run_stress_test "Unset multiple variables" \
    "export A=1 B=2 C=3\nunset A B\necho \$A \$B \$C\nexit" \
    "true"

run_stress_test "Echo with escape sequences" \
    "echo -e 'hello\\nworld\\ttab'\nexit" \
    "false"  # Echo -e not implemented

# ========== ERROR HANDLING TESTS ==========
echo -e "\n${BLUE}--- Error Handling Tests ---${NC}"

run_stress_test "Command not found" \
    "nonexistent_command\necho \$?\nexit" \
    "true"

run_stress_test "Permission denied" \
    "touch /tmp/testfile\nchmod 000 /tmp/testfile\n/tmp/testfile\necho \$?\nrm /tmp/testfile\nexit" \
    "false"  # Complex permission handling

run_stress_test "Broken pipe handling" \
    "yes | head -1\nexit" \
    "false"  # 'yes' command not available, SIGPIPE handling

# ========== WHITESPACE AND SPECIAL CHARACTERS ==========
echo -e "\n${BLUE}--- Whitespace and Special Character Tests ---${NC}"

run_stress_test "Multiple spaces and tabs" \
    "echo    hello     world   \nexit" \
    "true"

run_stress_test "Special characters in filenames" \
    "echo 'test' > '/tmp/file with spaces.txt'\ncat '/tmp/file with spaces.txt'\nrm '/tmp/file with spaces.txt'\nexit" \
    "true"

run_stress_test "Empty command lines" \
    "\n\n   \n\necho hello\nexit" \
    "true"

# ========== SIGNAL HANDLING TESTS (Interactive) ==========
echo -e "\n${BLUE}--- Signal Handling Tests ---${NC}"
echo -e "${PURPLE}Note: These tests check signal handling but may require manual verification${NC}"

run_functional_test "SIGINT handling in pipeline" \
    "sleep 10 | cat" \
    "should handle Ctrl+C gracefully"

# ========== MEMORY AND RESOURCE TESTS ==========
echo -e "\n${BLUE}--- Memory and Resource Tests ---${NC}"

run_stress_test "Very long command line" \
    "echo $(printf 'a%.0s' {1..1000})\nexit" \
    "false"  # Command substitution not implemented

run_stress_test "Many environment variables" \
    "$(for i in {1..50}; do echo \"export VAR$i=$i\"; done)\nenv | wc -l\nexit" \
    "false"  # Command substitution and wc

# ========== ADVANCED BASH FEATURES (Expected to fail) ==========
echo -e "\n${BLUE}--- Advanced Bash Features (Expected Failures) ---${NC}"

run_stress_test "Command substitution" \
    "echo \"Today is \$(date)\"\nexit" \
    "false"

run_stress_test "Arithmetic expansion" \
    "echo \$((2 + 3))\nexit" \
    "false"

run_stress_test "Process substitution" \
    "diff <(echo hello) <(echo world)\nexit" \
    "false"

run_stress_test "Brace expansion" \
    "echo {1..5}\nexit" \
    "false"

run_stress_test "Globbing" \
    "echo *.sh\nexit" \
    "false"

run_stress_test "Background processes" \
    "sleep 1 &\necho done\nexit" \
    "false"

run_stress_test "Job control" \
    "jobs\nexit" \
    "false"

# ========== TEST SUMMARY ==========
echo -e "\n${PURPLE}=== Advanced Test Summary ===${NC}"
echo "Tests run: $test_count"
echo "Tests passed: $passed_count"
echo "Tests failed: $((test_count - passed_count))"
echo "Expected failures (unimplemented features): $expected_failures"
echo "Unexpected failures: $((test_count - passed_count - expected_failures))"

if [ $((test_count - passed_count - expected_failures)) -eq 0 ]; then
    echo -e "${GREEN}🎉 All implemented features work correctly!${NC}"
    echo -e "${BLUE}📝 $expected_failures features are not implemented (as expected)${NC}"
    exit 0
else
    echo -e "${RED}❌ Some implemented features have issues.${NC}"
    echo -e "Success rate for implemented features: $(((passed_count * 100) / (test_count - expected_failures)))%"
    exit 1
fi
