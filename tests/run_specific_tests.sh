#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run_specific_tests.sh                              :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcos <marcos@student.42madrid.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/28 15:00:00 by marcos            #+#    #+#              #
#    Updated: 2025/08/28 15:00:00 by marcos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Script to run specific test categories

source "$(dirname "$0")/comprehensive_test.sh"

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

usage() {
    echo -e "${BLUE}Usage: $0 [OPTIONS] [TEST_CATEGORY]${NC}"
    echo ""
    echo "Test Categories:"
    echo "  basic       - Basic functionality tests"
    echo "  builtin     - Built-in command tests"
    echo "  pipeline    - Pipeline functionality tests"
    echo "  redirect    - Redirection tests"
    echo "  heredoc     - Heredoc tests"
    echo "  error       - Error handling tests"
    echo "  signal      - Signal handling tests"
    echo "  memory      - Memory leak tests (requires valgrind)"
    echo "  performance - Performance tests"
    echo "  edge        - Edge case tests"
    echo "  integration - Integration tests"
    echo "  all         - All tests (default)"
    echo ""
    echo "Options:"
    echo "  -v, --verbose   Verbose output"
    echo "  -q, --quiet     Quiet mode (only results)"
    echo "  -l, --list      List available test categories"
    echo "  -h, --help      Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0 basic          # Run basic tests only"
    echo "  $0 memory         # Run memory leak tests"
    echo "  $0 --verbose all  # Run all tests with verbose output"
}

list_categories() {
    echo -e "${BLUE}Available test categories:${NC}"
    echo "  basic       - Test basic shell functionality (echo, pwd, etc.)"
    echo "  builtin     - Test built-in commands (cd, export, env, etc.)"
    echo "  pipeline    - Test pipeline functionality (|)"
    echo "  redirect    - Test input/output redirection (<, >, >>)"
    echo "  heredoc     - Test heredoc functionality (<<)"
    echo "  error       - Test error handling and invalid input"
    echo "  signal      - Test signal handling (SIGINT, SIGTERM, etc.)"
    echo "  memory      - Test for memory leaks (requires valgrind)"
    echo "  performance - Test execution performance and timeouts"
    echo "  edge        - Test edge cases and boundary conditions"
    echo "  integration - Test complex scenarios combining features"
    echo "  all         - Run all available tests"
}

run_specific_category() {
    local category="$1"
    
    echo -e "${BLUE}Running $category tests...${NC}\n"
    
    # Setup test environment
    setup_test_env
    
    case "$category" in
        "basic")
            run_basic_tests
            ;;
        "builtin")
            run_builtin_tests
            ;;
        "pipeline")
            run_pipeline_tests
            ;;
        "redirect")
            run_redirection_tests
            ;;
        "heredoc")
            run_heredoc_tests
            ;;
        "error")
            run_error_handling_tests
            ;;
        "signal")
            run_signal_tests
            ;;
        "memory")
            run_memory_tests
            ;;
        "performance")
            run_performance_tests
            ;;
        "edge")
            run_edge_case_tests
            ;;
        "integration")
            run_integration_tests
            ;;
        "all")
            main
            return
            ;;
        *)
            echo -e "${RED}Error: Unknown test category '$category'${NC}"
            echo ""
            usage
            exit 1
            ;;
    esac
    
    # Print results for specific category
    echo -e "\n${BLUE}=== CATEGORY RESULTS ===${NC}"
    echo -e "Total Tests:   ${WHITE}$TOTAL_TESTS${NC}"
    echo -e "Passed:        ${GREEN}$PASSED_TESTS${NC}"
    echo -e "Failed:        ${RED}$FAILED_TESTS${NC}"
    echo -e "Skipped:       ${YELLOW}$SKIPPED_TESTS${NC}"
    
    if [ $FAILED_TESTS -eq 0 ]; then
        echo -e "\n${GREEN}✓ All $category tests passed!${NC}"
        exit 0
    else
        echo -e "\n${RED}✗ Some $category tests failed!${NC}"
        exit 1
    fi
}

# Parse command line arguments
VERBOSE=false
QUIET=false
CATEGORY="all"

while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -q|--quiet)
            QUIET=true
            shift
            ;;
        -l|--list)
            list_categories
            exit 0
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        -*)
            echo -e "${RED}Error: Unknown option $1${NC}"
            usage
            exit 1
            ;;
        *)
            CATEGORY="$1"
            shift
            ;;
    esac
done

# Adjust output based on flags
if $QUIET; then
    exec > /dev/null 2>&1
elif $VERBOSE; then
    set -x
fi

# Validate category
case "$CATEGORY" in
    basic|builtin|pipeline|redirect|heredoc|error|signal|memory|performance|edge|integration|all)
        run_specific_category "$CATEGORY"
        ;;
    *)
        echo -e "${RED}Error: Invalid test category '$CATEGORY'${NC}"
        echo ""
        usage
        exit 1
        ;;
esac
