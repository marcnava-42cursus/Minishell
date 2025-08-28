#!/bin/bash

echo "Testing pipes in minishell..."
echo ""

# Test 1: Simple pipe
echo "Test 1: Simple pipe - echo hello | cat"
echo "echo hello | cat" | ./minishell

echo ""

# Test 2: Multiple pipes
echo "Test 2: Multiple pipes - echo hello world | grep hello | cat"
echo "echo hello world | grep hello | cat" | ./minishell

echo ""

# Test 3: With ls and grep
echo "Test 3: List and filter - ls | grep .c"
echo "ls | grep .c" | ./minishell

echo ""

echo "Pipe tests completed!"
