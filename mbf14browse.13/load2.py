import os
import numpy as np
import csv

output_rows = []

for n in range(4098):
    filename = f"mbf.{n}.txt"
    loop_data = {}
    with open(filename, 'r') as f:
        for line in f:
            if line.startswith("runtime:"):
                continue
            parts = line.strip().split('\t')
            if len(parts) == 3:
                loop = int(parts[0])
                idx = int(parts[1])
                value = float(parts[2])

                if loop not in loop_data:
                    loop_data[loop] = [None, None]  # [left, right]

                if idx == n:
                    loop_data[loop][0] = value
                elif idx == n + 1:
                    loop_data[loop][1] = value
                else:
                    raise ValueError(f"Unexpected index {idx} in file {filename}")

    # Extract left and right values as lists
    left_values = [v[0] for v in loop_data.values() if v[0] is not None]
    right_values = [v[1] for v in loop_data.values() if v[1] is not None]

    # Compute statistics
    mean_p_left = np.mean(left_values)
    std_p_left = np.std(left_values)
    mean_p_right = np.mean(right_values)
    std_p_right = np.std(right_values)

    # Store row: left, right, mean_p_left, std_p_left, mean_p_right, std_p_right
    output_rows.append([n, n+1, mean_p_left, std_p_left, mean_p_right, std_p_right])

# Write to CSV
with open("mbf_summary.csv", "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["left", "right", "mean_p_left", "std_p_left", "mean_p_right", "std_p_right"])
    writer.writerows(output_rows)

print("Summary written to mbf_summary.csv")
