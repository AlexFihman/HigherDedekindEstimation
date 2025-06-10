import os

data = {}  # Will store data as {N: {loop: (left_value, right_value)}}

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

                # Initialize the loop entry if not present
                if loop not in loop_data:
                    loop_data[loop] = [None, None]  # [left, right]

                if idx == n:
                    loop_data[loop][0] = value  # left
                elif idx == n + 1:
                    loop_data[loop][1] = value  # right
                else:
                    raise ValueError(f"Unexpected index {idx} in file {filename}")
    data[n] = loop_data

print(f"Loaded {len(data)} files.")
