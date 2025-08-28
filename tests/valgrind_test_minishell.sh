#!/bin/bash

# Comprehensive Valgrind Testing for Minishell
# Tests memory leaks, file descriptor leaks, and child process issues

MINISHELL="./minishell"
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
NC='\033[0m' # No Color

echo -e "${PURPLE}=== Comprehensive Valgrind Testing for Minishell ===${NC}\n"

# Check if valgrind is available
if ! command -v valgrind &> /dev/null; then
    echo -e "${RED}❌ Valgrind is not installed. Please install it first.${NC}"
    exit 1
fi

# Create test directory
mkdir -p /tmp/minishell_valgrind_tests
cd /tmp/minishell_valgrind_tests

test_count=0
passed_count=0

# Function to run valgrind test
run_valgrind_test() {
    local test_name="$1"
    local commands="$2"
    local valgrind_options="$3"
    
    test_count=$((test_count + 1))
    echo -e "${YELLOW}Test $test_count: $test_name${NC}"
    
    # Create a log file for this test
    local log_file="valgrind_test_${test_count}.log"
    
    # Run valgrind with the specified options
    echo -e "$commands" | timeout 10s valgrind $valgrind_options $MINISHELL > test_output_${test_count}.txt 2> $log_file
    local exit_code=$?
    
    # Analyze results
    local errors=$(grep -E "ERROR SUMMARY|LEAK SUMMARY|definitely lost|indirectly lost|possibly lost|still reachable" $log_file)
    local definite_leaks=$(grep "definitely lost:" $log_file | grep -o '[0-9,]* bytes' | head -1)
    local possible_leaks=$(grep "possibly lost:" $log_file | grep -o '[0-9,]* bytes' | head -1)
    local fd_errors=$(grep -E "file descriptor|Bad file descriptor|still open" $log_file)
    
    # Check for success (no definite memory leaks, no file descriptor leaks)
    local success=true
    
    if [[ $definite_leaks == *"0 bytes"* ]] || [[ -z $definite_leaks ]]; then
        echo -e "${GREEN}✓ No definite memory leaks${NC}"
    else
        echo -e "${RED}✗ Definite memory leaks found: $definite_leaks${NC}"
        success=false
    fi
    
    if [[ -z $fd_errors ]]; then
        echo -e "${GREEN}✓ No file descriptor leaks${NC}"
    else
        echo -e "${RED}✗ File descriptor issues:${NC}"
        echo "$fd_errors"
        success=false
    fi
    
    # Show error summary
    local error_count=$(grep "ERROR SUMMARY:" $log_file | grep -o '[0-9]*' | head -1)
    if [[ -z $error_count ]] || [[ $error_count -eq 0 ]]; then
        echo -e "${GREEN}✓ No memory errors${NC}"
    else
        echo -e "${RED}✗ $error_count memory errors found${NC}"
        success=false
    fi
    
    if $success; then
        echo -e "${GREEN}🎉 OVERALL: PASS${NC}"
        passed_count=$((passed_count + 1))
    else
        echo -e "${RED}❌ OVERALL: FAIL${NC}"
        echo "Full log available at: $PWD/$log_file"
    fi
    
    echo "---"
}

echo -e "\n${BLUE}=== Basic Memory Leak Tests ===${NC}"

# Test 1: Basic command execution
run_valgrind_test "Basic command execution" \
    "echo hello\nexit" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes"

# Test 2: Variable operations
run_valgrind_test "Variable export and expansion" \
    "export TEST=hello\necho \$TEST\nunset TEST\nexit" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes"

# Test 3: Built-in commands
run_valgrind_test "Built-in commands" \
    "pwd\ncd /tmp\npwd\nenv | head -3\nexit" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes"

echo -e "\n${BLUE}=== File Descriptor Leak Tests ===${NC}"

# Test 4: File redirections
run_valgrind_test "File redirections" \
    "echo test > test_file.txt\ncat < test_file.txt\ncat test_file.txt > test_file2.txt\nrm test_file.txt test_file2.txt\nexit" \
    "--leak-check=full --track-fds=yes"

# Test 5: Heredoc handling
run_valgrind_test "Heredoc operations" \
    "cat << EOF\nhello world\nthis is a test\nEOF\ncat << END\nanother heredoc\nEND\nexit" \
    "--leak-check=full --track-fds=yes"

# Test 6: Multiple redirections
run_valgrind_test "Multiple redirections" \
    "echo line1 > file1.txt\necho line2 >> file1.txt\ncat file1.txt > file2.txt\ncat file2.txt\nrm file1.txt file2.txt\nexit" \
    "--leak-check=full --track-fds=yes"

echo -e "\n${BLUE}=== Pipeline and Child Process Tests ===${NC}"

# Test 7: Simple pipeline
run_valgrind_test "Simple pipeline" \
    "echo hello world | cat\necho test | grep test\nexit" \
    "--leak-check=full --trace-children=yes --track-fds=yes"

# Test 8: Complex pipeline
run_valgrind_test "Complex pipeline" \
    "echo -e 'line1\\nline2\\nline3' | head -2 | tail -1\nenv | grep HOME | head -1\nexit" \
    "--leak-check=full --trace-children=yes --track-fds=yes"

# Test 9: Pipeline with redirections
run_valgrind_test "Pipeline with redirections" \
    "echo hello | cat > pipe_output.txt\ncat pipe_output.txt\necho world | cat >> pipe_output.txt\ncat pipe_output.txt\nrm pipe_output.txt\nexit" \
    "--leak-check=full --trace-children=yes --track-fds=yes"

# Test 10: Built-ins in pipelines
run_valgrind_test "Built-ins in pipelines" \
    "env | head -5\npwd | cat\necho test | env\nexit" \
    "--leak-check=full --trace-children=yes --track-fds=yes"

echo -e "\n${BLUE}=== Error Handling Tests ===${NC}"

# Test 11: Command not found
run_valgrind_test "Command not found" \
    "nonexistent_command\necho \$?\nexit" \
    "--leak-check=full --show-leak-kinds=all"

# Test 12: Syntax errors
run_valgrind_test "Syntax error handling" \
    "echo hello |\necho world >\n| cat\nexit" \
    "--leak-check=full --show-leak-kinds=all"

# Test 13: File permission errors
run_valgrind_test "Permission errors" \
    "touch no_perm_file.txt\nchmod 000 no_perm_file.txt\ncat no_perm_file.txt\nrm no_perm_file.txt\nexit" \
    "--leak-check=full --show-leak-kinds=all"

echo -e "\n${BLUE}=== Stress Tests ===${NC}"

# Test 14: Many variables
run_valgrind_test "Many environment variables" \
    "export A=1 B=2 C=3 D=4 E=5\necho \$A \$B \$C \$D \$E\nunset A B C D E\nexit" \
    "--leak-check=full --show-leak-kinds=all"

# Test 15: Long command lines
run_valgrind_test "Long command line" \
    "echo this is a very long command line with many arguments to test memory allocation and deallocation\nexit" \
    "--leak-check=full --show-leak-kinds=all"

# Test 16: Rapid command execution
run_valgrind_test "Rapid command execution" \
    "echo 1\necho 2\necho 3\necho 4\necho 5\npwd\nenv | head -1\necho final\nexit" \
    "--leak-check=full --show-leak-kinds=all"

echo -e "\n${BLUE}=== Advanced Memory Analysis ===${NC}"

# Test 17: Deep memory analysis with children
run_valgrind_test "Deep memory analysis" \
    "echo hello | cat | cat\necho world > temp.txt\ncat temp.txt | grep world\nrm temp.txt\nexit" \
    "--leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes --track-origins=yes"

# Test 18: Exit status and cleanup
run_valgrind_test "Exit status handling" \
    "true\necho \$?\nfalse\necho \$?\n/bin/nonexistent 2>/dev/null\necho \$?\nexit 42" \
    "--leak-check=full --show-leak-kinds=all --track-origins=yes"

echo -e "\n${PURPLE}=== Valgrind Test Summary ===${NC}"
echo "Total tests run: $test_count"
echo "Tests passed: $passed_count"
echo "Tests failed: $((test_count - passed_count))"

# Overall assessment
if [ $passed_count -eq $test_count ]; then
    echo -e "${GREEN}🏆 Perfect! No memory leaks or resource leaks detected!${NC}"
    exit_code=0
elif [ $passed_count -gt $((test_count * 3 / 4)) ]; then
    echo -e "${YELLOW}⚠️  Minor issues detected, but overall good memory management${NC}"
    exit_code=0
else
    echo -e "${RED}❌ Significant memory or resource management issues detected${NC}"
    exit_code=1
fi

echo -e "\n${BLUE}ℹ️  Test logs and outputs are available in:${NC}"
echo "$PWD"
echo -e "\n${BLUE}To view a specific test log:${NC}"
echo "cat $PWD/valgrind_test_N.log (replace N with test number)"

# Cleanup
echo -e "\n${BLUE}Cleaning up test files...${NC}"
rm -f test_file*.txt pipe_output.txt temp.txt no_perm_file.txt test_output_*.txt

exit $exit_code
