#!/usr/bin/env bash


FINAL_BB_FEATURES_FILE="bigBBFeatures.csv"
FINAL_FN_FEATURES_FILE="bigFNFeatures.csv"

FN_CSV_HEADER="Function ID;Function Name;Instructions;BBs;In-degree;Out-degree;Static Allocations;Dynamic Allocations;MemOps;CondBranches;UnCondBranches;DirectCalls;InDirectCalls;VULNERABLE"

BB_CSV_HEADER="Block ID;Block Name;Instructions;In-degree;Out-degree;Static Allocations;Dynamic Allocations;MemOps;CondBranches;UnCondBranches;DirectCalls;InDirectCalls;VULNERABLE"

echo "$BB_CSV_HEADER" >> $FINAL_BB_FEATURES_FILE
find ./CSVS/ -type f -name "*_BBFeatures.csv" | while read -r bbfile; do
    cat "$bbfile" >> $FINAL_BB_FEATURES_FILE
    rm "$bbfile"
done

echo "$FN_CSV_HEADER" >> $FINAL_FN_FEATURES_FILE
find ./CSVS/ -type f -name "*_FunctionFeatues.csv" | while read -r fnfile; do
    cat "$fnfile" >> $FINAL_FN_FEATURES_FILE
    rm "$fnfile"
done
