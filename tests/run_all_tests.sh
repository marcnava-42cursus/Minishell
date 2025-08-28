#!/bin/bash

# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    run_all_tests.sh                                   :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marcos <marcos@student.42madrid.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/08/28 15:00:00 by marcos            #+#    #+#              #
#    Updated: 2025/08/28 15:00:00 by marcos           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Master script to run all available tests

set -e

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
PURPLE='\033[0;35m'
CYAN='\033[0;36m'
WHITE='\033[1;37m'
NC='\033[0m'

# Get script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Test results
COMPREHENSIVE_RESULT=0
COMPARISON_RESULT=0
LEGACY_RESULT=0

echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${WHITE}              MINISHELL COMPLETE TEST SUITE                 ${NC}"
echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"

# Change to project directory
cd "$PROJECT_DIR"

# Check if minishell exists
if [ ! -f "./minishell" ]; then
    echo -e "${YELLOW}Minishell binary not found. Compiling...${NC}"
    make
    echo ""
fi

echo -e "${BLUE}Starting complete test suite...${NC}"
echo -e "Project directory: $PROJECT_DIR"
echo -e "Tests directory: $SCRIPT_DIR"
echo ""

# 1. Run comprehensive tests
echo -e "${PURPLE}▶ Running Comprehensive Tests...${NC}"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if "$SCRIPT_DIR/comprehensive_test.sh"; then
    echo -e "${GREEN}✓ Comprehensive tests PASSED${NC}"
    COMPREHENSIVE_RESULT=0
else
    echo -e "${RED}✗ Comprehensive tests FAILED${NC}"
    COMPREHENSIVE_RESULT=1
fi
echo ""

# 2. Run bash comparison
echo -e "${CYAN}▶ Running Bash Compatibility Tests...${NC}"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if "$SCRIPT_DIR/compare_with_bash.sh"; then
    echo -e "${GREEN}✓ Bash compatibility tests PASSED${NC}"
    COMPARISON_RESULT=0
else
    echo -e "${RED}✗ Bash compatibility tests FAILED${NC}"
    COMPARISON_RESULT=1
fi
echo ""

# 3. Run legacy tests for backward compatibility
echo -e "${YELLOW}▶ Running Legacy Tests...${NC}"
echo "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━"
if "$SCRIPT_DIR/test_minishell.sh"; then
    echo -e "${GREEN}✓ Legacy tests PASSED${NC}"
    LEGACY_RESULT=0
else
    echo -e "${RED}✗ Legacy tests FAILED${NC}"
    LEGACY_RESULT=1
fi
echo ""

# Calculate overall results
TOTAL_FAILED=$((COMPREHENSIVE_RESULT + COMPARISON_RESULT + LEGACY_RESULT))
TOTAL_TESTS=3
PASSED_TESTS=$((TOTAL_TESTS - TOTAL_FAILED))

# Final summary
echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"
echo -e "${WHITE}                      FINAL SUMMARY                         ${NC}"
echo -e "${WHITE}═══════════════════════════════════════════════════════════${NC}"

echo -e "Test Suites Run:        ${WHITE}$TOTAL_TESTS${NC}"
echo -e "Test Suites Passed:     ${GREEN}$PASSED_TESTS${NC}"
echo -e "Test Suites Failed:     ${RED}$TOTAL_FAILED${NC}"
echo ""

echo -e "📊 Individual Results:"
if [ $COMPREHENSIVE_RESULT -eq 0 ]; then
    echo -e "   Comprehensive Tests:  ${GREEN}✓ PASSED${NC}"
else
    echo -e "   Comprehensive Tests:  ${RED}✗ FAILED${NC}"
fi

if [ $COMPARISON_RESULT -eq 0 ]; then
    echo -e "   Bash Compatibility:   ${GREEN}✓ PASSED${NC}"
else
    echo -e "   Bash Compatibility:   ${RED}✗ FAILED${NC}"
fi

if [ $LEGACY_RESULT -eq 0 ]; then
    echo -e "   Legacy Tests:         ${GREEN}✓ PASSED${NC}"
else
    echo -e "   Legacy Tests:         ${RED}✗ FAILED${NC}"
fi

echo ""

# Final verdict
if [ $TOTAL_FAILED -eq 0 ]; then
    echo -e "${GREEN}🎉 ALL TEST SUITES PASSED! 🎉${NC}"
    echo -e "${GREEN}Your minishell implementation is excellent!${NC}"
    echo -e "${GREEN}Ready for submission! 🚀${NC}"
    exit 0
elif [ $TOTAL_FAILED -eq 1 ]; then
    echo -e "${YELLOW}⚠️  ONE TEST SUITE FAILED ⚠️${NC}"
    echo -e "${YELLOW}Your implementation is mostly working but needs some fixes.${NC}"
    exit 1
else
    echo -e "${RED}❌ MULTIPLE TEST SUITES FAILED ❌${NC}"
    echo -e "${RED}Your implementation needs significant work.${NC}"
    exit 1
fi
