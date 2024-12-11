#!/usr/bin/env bash

FINAL_BB_FEATURES_FILE="bigBBFeatures.csv"
FINAL_FN_FEATURES_FILE="bigFNFeatures.csv"

FN_CSV_HEADER="Function ID;Function Name;Instructions;BBs;In-degree;Out-degree;Num Loops;Static Allocations;Dynamic Allocations;MemOps;CondBranches;UnCondBranches;DirectCalls;InDirectCalls;VULNERABLE"

BB_CSV_HEADER="Block ID;Block Name;Instructions;In-degree;Out-degree;Static Allocations;Dynamic Allocations;MemOps;CondBranches;UnCondBranches;DirectCalls;InDirectCalls;VULNERABLE"

# Add header to BB features file
echo "$BB_CSV_HEADER" > "$FINAL_BB_FEATURES_FILE"

# Find all BB feature CSV files and count them for progress bar
bb_files=$(find ./CSVS/ -type f -name "*_BBFeatures.csv")
total_bb_files=$(echo "$bb_files" | wc -l)

# Concatenate BB feature CSV files with progress bar
counter=0
echo "$bb_files" | while read -r bbfile; do
    counter=$((counter + 1))
    # Concatenate file contents (no header to skip)
    cat "$bbfile" >> "$FINAL_BB_FEATURES_FILE"
    rm "$bbfile"  # Remove the original file
    # Display progress bar
    echo -n "$counter/$total_bb_files files processed for BB features... "
    printf "[%-50s] %d%%\r" "$(printf "%0.s#" $(seq 1 $((counter * 50 / total_bb_files))))" "$((counter * 100 / total_bb_files))"
done
echo -e "\nFinished processing BB feature files."

# Add header to FN features file
echo "$FN_CSV_HEADER" > "$FINAL_FN_FEATURES_FILE"

# Find all FN feature CSV files and count them for progress bar
fn_files=$(find ./CSVS/ -type f -name "*_FunctionFeatures.csv")
total_fn_files=$(echo "$fn_files" | wc -l)

# Concatenate FN feature CSV files with progress bar
counter=0
echo "$fn_files" | while read -r fnfile; do
    counter=$((counter + 1))
    # Concatenate file contents (no header to skip)
    cat "$fnfile" >> "$FINAL_FN_FEATURES_FILE"
    rm "$fnfile"  # Remove the original file
    # Display progress bar
    echo -n "$counter/$total_fn_files files processed for FN features... "

    printf "[%-50s] %d%%\r" "$(printf "%0.s#" $(seq 1 $((counter * 50 / total_fn_files))))" "$((counter * 100 / total_fn_files))"
done
echo -e "\nFinished processing FN feature files."

echo "CSV files have been successfully concatenated and original files removed."
