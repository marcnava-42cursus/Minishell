#!/bin/bash

output=$(norminette --use-gitignore -R CheckForbidenSourceHeader)

ok_files=$(echo "$output" | grep "OK!" | wc -l)
error_files=$(echo "$output" | grep "Error!" | wc -l)

echo "Correct files: $ok_files"
echo "Incorrect files: $error_files"

# Extract error lines only
error_lines=$(echo "$output" | grep "Error:")

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
