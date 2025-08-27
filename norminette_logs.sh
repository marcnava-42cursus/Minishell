#!/bin/bash

output=$(norminette -R CheckForbidenSourceHeader --use-gitignore)

# Function to show usage
show_usage() {
    echo "Usage: $0 [ERROR_TYPE1] [ERROR_TYPE2] ..."
    echo "Example: $0 FORBIDDEN INVALID_HEADER"
    echo "This will match all errors containing FORBIDDEN or INVALID_HEADER"
    echo ""
}

# Check if help is requested
if [[ "$1" == "-h" || "$1" == "--help" ]]; then
    show_usage
    exit 0
fi

ok_files=$(echo "$output" | grep "OK!" | wc -l)
error_files=$(echo "$output" | grep "Error!" | wc -l)

echo "Correct files: $ok_files"
echo "Incorrect files: $error_files"

# Extract error lines only
error_lines=$(echo "$output" | grep "Error:")

# If arguments are provided, filter by those error types
if [ $# -gt 0 ]; then
    echo ""
    echo "Filtering by error types: $@"
    echo "========================================"
    
    # Create a grep pattern for all provided arguments (case insensitive partial match)
    filter_pattern=""
    for arg in "$@"; do
        if [ -z "$filter_pattern" ]; then
            filter_pattern="$arg"
        else
            filter_pattern="$filter_pattern\|$arg"
        fi
    done
    
    # Filter error lines by the pattern
    filtered_errors=$(echo "$error_lines" | grep -i "$filter_pattern")
    
    if [ -z "$filtered_errors" ]; then
        echo "No errors found matching the specified patterns."
        exit 0
    fi
    
    # Group by file and show errors with line numbers
    echo "$filtered_errors" | while IFS= read -r line; do
        # Extract file path, line number, and error type
        file_path=$(echo "$line" | sed 's/:\([0-9]*\):.*//')
        line_number=$(echo "$line" | sed 's/.*:\([0-9]*\):.*/\1/')
        error_type=$(echo "$line" | sed 's/.*Error: \([A-Z_]*\).*/\1/')
        error_detail=$(echo "$line" | sed 's/.*Error: [A-Z_]* \(.*\)/\1/')
        
        printf "\n%s:%s - %s\n" "$file_path" "$line_number" "$error_type"
        printf "  └─ %s\n" "$error_detail"
    done
    
    # Show statistics for filtered errors
    echo ""
    echo "Filtered error statistics:"
    echo "-------------------------"
    echo "$filtered_errors" | sed -e 's/.*Error: \([A-Z_]*\).*/\1/g' | sort | uniq -c | sort -nr | while read count error_type; do
        printf "%-25s: %s\n" "$error_type" "$count"
    done
    
    filtered_count=$(echo "$filtered_errors" | wc -l)
    echo ""
    echo "Total filtered errors: $filtered_count"
    
else
    # Original behavior when no arguments are provided
    # Count occurrences of each error type
    echo ""
    echo "Error type statistics:"
    echo "--------------------"
    echo "$error_lines" | sed -e 's/.*Error: \([A-Z_]*\).*/\1/g' | sort | uniq -c | sort -nr | while read count error_type; do
        printf "%-25s: %s\n" "$error_type" "$count"
    done
    
    # Display total error count
    total_errors=$(echo "$error_lines" | wc -l)
    echo ""
    echo "Total errors: $total_errors"
fi
