#!/bin/bash

# Folder where the 'walk' program is located
program_folder="."

# Total number of batches
total_batches=4097

# Maximum number of parallel jobs
max_parallel_jobs=16

# Batch execution loop
for ((batch_no=0; batch_no<=total_batches; batch_no++)); do
    # Run the program in the background
    end_batch=$((batch_no + 1))
    "$program_folder/mbf14browse" "100000000" "$batch_no" "$end_batch" &
     
    # Limit to max_parallel_jobs
    if (( batch_no % max_parallel_jobs == 0 )); then
        wait  # Wait for all background jobs to finish
    fi
done

# Ensure all remaining jobs finish
wait

echo "All batches completed."
