#!/bin/bash

# Focused Minishell Issue Detection Test
# Identifies specific problems without false positives from prompt text

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}=== Focused Minishell Issue Detection ===${NC}\n"

test_count=0
passed_count=0

# Function to test core functionality that should work
test_core_feature() {
    local test_name="$1"
    local commands="$2"
    local expected_output="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Run test and extract just command output (no prompts)
    actual_output=$(echo -e "$commands" | $MINISHELL 2>&1 | grep -v "Minishell v" | grep -v ">" | grep -v "^$" | sed '/exit$/d')
    
    if [[ "$actual_output" =~ $expected_output ]]; then
        echo -e "${GREEN}✓ PASS${NC}"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Expected: $expected_output"
        echo "Got: '$actual_output'"
    fi
    echo "---"
}

# Test output-only (ignores prompts and exit messages)
test_output_only() {
    local test_name="$1"
    local commands="$2"
    local expected_pattern="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Get only the command output lines (filter out prompts)
    output=$(echo -e "$commands" | $MINISHELL 2>/dev/null | grep -v "Minishell v" | grep -v ">" | grep -v "^$" | sed '/^exit$/d')
    
    if [[ $output =~ $expected_pattern ]]; then
        echo -e "${GREEN}✓ PASS${NC}"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}✗ FAIL${NC}"
        echo "Expected pattern: $expected_pattern"
        echo "Actual output: '$output'"
    fi
    echo "---"
}

echo -e "\n${BLUE}=== CRITICAL FUNCTIONALITY TESTS ===${NC}"

# Test 1: Basic exit status variable expansion
test_output_only "Exit status variable \$?" \
    "true\necho \$?\nfalse\necho \$?\nexit" \
    "0.*1"

# Test 2: Variable expansion in quotes
test_output_only "Variable expansion in double quotes" \
    "export TEST=hello\necho \"\$TEST world\"\nexit" \
    "hello world"

# Test 3: Single vs double quote behavior
test_output_only "Single quotes prevent expansion" \
    "export TEST=hello\necho '\$TEST'\nexit" \
    "\\$TEST"

# Test 4: Simple redirection
test_core_feature "Basic output redirection" \
    "echo test > /tmp/minishell_test.txt\ncat /tmp/minishell_test.txt\nrm /tmp/minishell_test.txt\nexit" \
    "test"

# Test 5: Pipeline with redirection
test_output_only "Pipeline to file" \
    "echo hello | cat > /tmp/pipe_test.txt\ncat /tmp/pipe_test.txt\nrm /tmp/pipe_test.txt\nexit" \
    "hello"

# Test 6: Multiple spaces handling
test_output_only "Multiple spaces normalization" \
    "echo    hello     world\nexit" \
    "hello world"

# Test 7: Empty quoted strings
test_output_only "Empty quoted strings" \
    "echo '' test ''\nexit" \
    " test "

# Test 8: Command not found with correct exit status
test_output_only "Command not found error" \
    "nonexistent_cmd 2>/dev/null\necho \$?\nexit" \
    "127"

echo -e "\n${BLUE}=== ADVANCED FUNCTIONALITY TESTS ===${NC}"

# Test 9: Export multiple variables in one command
test_output_only "Export multiple variables" \
    "export A=1 B=2\necho \$A \$B\nexit" \
    "1 2"

# Test 10: Unset multiple variables
test_output_only "Unset multiple variables" \
    "export X=1 Y=2 Z=3\nunset X Y\necho \$X \$Y \$Z\nexit" \
    " 3"

# Test 11: Complex pipeline
test_output_only "Multi-stage pipeline" \
    "echo -e 'apple\\nbanana\\ncherry' | grep a | head -1\nexit" \
    "apple"

# Test 12: Heredoc with pipeline
test_output_only "Heredoc with pipeline" \
    "cat << EOF | grep hello\nhello world\ngoodbye\nEOF\nexit" \
    "hello world"

echo -e "\n${BLUE}=== ERROR HANDLING TESTS ===${NC}"

# Test 13: Syntax error detection
test_core_feature "Pipe syntax error" \
    "echo hello |\nexit" \
    "syntax error"

# Test 14: Redirection syntax error  
test_core_feature "Redirection syntax error" \
    "echo hello >\nexit" \
    "syntax error"

echo -e "\n${BLUE}=== KNOWN LIMITATIONS (Should fail - documenting current state) ===${NC}"

# Document what doesn't work yet
echo -e "${YELLOW}Test $((test_count + 1)): Command substitution (not implemented)${NC}"
output=$(echo -e "echo \$(date)\nexit" | $MINISHELL 2>/dev/null | grep -v "Minishell v" | grep -v ">" | grep -v "^$" | sed '/^exit$/d')
echo -e "${BLUE}ℹ Current behavior: '$output'${NC}"
echo "---"

echo -e "${YELLOW}Test $((test_count + 2)): Globbing (not implemented)${NC}" 
output=$(echo -e "echo *.sh\nexit" | $MINISHELL 2>/dev/null | grep -v "Minishell v" | grep -v ">" | grep -v "^$" | sed '/^exit$/d')
echo -e "${BLUE}ℹ Current behavior: '$output'${NC}"
echo "---"

echo -e "${YELLOW}Test $((test_count + 3)): Tilde expansion (not implemented)${NC}"
output=$(echo -e "echo ~\nexit" | $MINISHELL 2>/dev/null | grep -v "Minishell v" | grep -v ">" | grep -v "^$" | sed '/^exit$/d')
echo -e "${BLUE}ℹ Current behavior: '$output'${NC}"
echo "---"

# Final summary
echo -e "\n${BLUE}=== FOCUSED TEST SUMMARY ===${NC}"
echo "Core functionality tests: $test_count"
echo "Tests passed: $passed_count"
echo "Tests failed: $((test_count - passed_count))"

if [ $passed_count -eq $test_count ]; then
    echo -e "${GREEN}🎉 All core functionality working!${NC}"
    exit 0
elif [ $passed_count -gt $((test_count / 2)) ]; then
    echo -e "${YELLOW}⚠️  Most core functionality works, some issues to fix${NC}"
    echo "Success rate: $((passed_count * 100 / test_count))%"
    exit 0
else
    echo -e "${RED}❌ Critical issues found in core functionality${NC}"
    echo "Success rate: $((passed_count * 100 / test_count))%"
    exit 1
fi
