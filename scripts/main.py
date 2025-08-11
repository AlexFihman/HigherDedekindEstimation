import math
import numpy as np
from collections import defaultdict
from decimal import Decimal, getcontext

total_runtime = 0.0
idx_data = defaultdict(lambda: {'l_adj': [], 'r_adj': []})
idx_averages = {}
data_folder = "data.13"


def print_hi(name):
    dimension = 13
    num_files = 1 << dimension
    getcontext().prec = 20
    print(f"Data folder: {data_folder}")
    print(f"Dimension: {dimension}")
    mid_file = num_files // 2
    for i in range(num_files):
        readfile(i)

    if total_runtime < 60:
        print(f"Total runtime: {total_runtime: .2f} seconds")
    elif total_runtime < 3600:
        print(f"Total runtime: {total_runtime / 60: .2f} minutes")
    elif total_runtime < 86400:
        print(f"Total runtime: {total_runtime / 3600: .2f} hours")
    else:
        print(f"Total runtime: {total_runtime / 86400: .2f} days")

    calculate_statistics()

    # calculate values from 0 to middle
    values = [Decimal(0)] * (mid_file + 1)
    std_err_ratio_values_sum_sq = [0] * (mid_file + 1)
    values[0] = Decimal(1)
    # loop from 1 to mid_file
    for i in range(1, mid_file + 1):
        l_mean = idx_averages[i]['l_mean']
        l_std_err_ratio = idx_averages[i]['l_std_err_ratio']
        r_mean = idx_averages[i - 1]['r_mean']
        r_std_err_ratio = idx_averages[i - 1]['r_std_err_ratio']
        std_err_ratio_sq = l_std_err_ratio ** 2 + r_std_err_ratio ** 2
        values[i] = values[i - 1] * Decimal(r_mean) / Decimal(l_mean)
        std_err_ratio_values_sum_sq[i] = std_err_ratio_values_sum_sq[i - 1] + std_err_ratio_sq

    max_value, max_index = max((v, i) for i, v in enumerate(values))
    error_left_sq = std_err_ratio_values_sum_sq[max_index]

    # for i in range(0, mid_file + 1):
    #    print(f"{i}\t{values[i]: .8E}\t{std_err_ratio_values[i]: .8E}")

    # 0 - end, 1 - one before end, etc.
    values_reverse = [Decimal(0)] * (mid_file + 1)
    std_err_ratio_values_sum_sq_reverse = [0] * (mid_file + 1)
    values_reverse[0] = Decimal(1)

    # calculate values from end to middle, in reverse order
    for i in range(1, mid_file + 1):
        pos_from_end = num_files - i
        l_mean = idx_averages[pos_from_end + 1]['l_mean']
        l_std_err_ratio = idx_averages[pos_from_end + 1]['l_std_err_ratio']
        r_mean = idx_averages[pos_from_end]['r_mean']
        r_std_err_ratio = idx_averages[pos_from_end]['r_std_err_ratio']
        std_err_ratio_sq = l_std_err_ratio ** 2 + r_std_err_ratio ** 2
        values_reverse[i] = values_reverse[i - 1] * Decimal(l_mean) / Decimal(r_mean)
        std_err_ratio_values_sum_sq_reverse[i] = std_err_ratio_values_sum_sq_reverse[i - 1] + std_err_ratio_sq

    max_value, max_index = max((v, i) for i, v in enumerate(values_reverse))
    error_right_sq = std_err_ratio_values_sum_sq_reverse[max_index]

    error_sum_sq = error_left_sq + error_right_sq
    error = math.sqrt(error_sum_sq)

    # for i in range(0, mid_file + 1):
    #    print(f"{i}\t{values_reverse[i]: .8E}")

    dedekind_n = Decimal(0)
    for i in range(0, mid_file):
        dedekind_n += values[i]
        dedekind_n += values_reverse[i]
    dedekind_n += Decimal('0.5') * (values[mid_file] + values_reverse[mid_file])
    print(f"Dedekind Number of {dimension} variables is: {dedekind_n: .8E} S.E.: {dedekind_n * Decimal(error): .8E}")
    print(f"Error between calculation starting from 0 and from the end: {abs(values[mid_file] / values_reverse[mid_file] - Decimal(1)): .8E}")


def readfile(n):
    global total_runtime
    global idx_data
    global data_folder
    filename = f"{data_folder}mbf.{n}.txt"
    try:
        with open(filename, 'r') as f:
            for line in f:
                try:
                    if line.startswith("runtime:"):
                        runtime_value = float(line.split(':')[1].strip())
                        total_runtime += runtime_value
                        continue
                    parts = line.strip().split('\t')
                    if len(parts) == 5:
                        idx = int(parts[1])
                        l_value = float(parts[2])
                        r_value = float(parts[3])
                        s_value = float(parts[4])

                        # Check for division by zero
                        if s_value != 0:
                            l_adj = l_value / s_value
                            r_adj = r_value / s_value

                            # Collect data for this idx
                            idx_data[idx]['l_adj'].append(l_adj)
                            idx_data[idx]['r_adj'].append(r_adj)
                        else:
                            print(f"Warning: Division by zero in file {filename}, line: {line.strip()}")
                except ValueError as e:
                    print(f"Warning: Invalid data format in file {filename}, line: {line.strip()}, error: {e}")
                except Exception as e:
                    print(f"Warning: Error processing line in file {filename}, line: {line.strip()}, error: {e}")
    except FileNotFoundError:
        print(f"Warning: File {filename} not found")
    except Exception as e:
        print(f"Error: Could not open file {filename}, error: {e}")


def calculate_statistics():
    global idx_data
    global idx_averages
    for idx, data in idx_data.items():
        l_adj_values = np.array(data['l_adj'])
        r_adj_values = np.array(data['r_adj'])

        count = len(l_adj_values)  # Should be same as len(r_adj_values)
        l_mean = np.mean(l_adj_values)
        l_std = np.std(l_adj_values)
        l_std_err = l_std / np.sqrt(count) if count > 0 else 0.0
        l_std_err_ratio = l_std_err / l_mean if l_mean != 0 else 0.0

        r_mean = np.mean(r_adj_values)
        r_std = np.std(r_adj_values)
        r_std_err = r_std / np.sqrt(count) if count > 0 else 0.0
        r_std_err_ratio = r_std_err / r_mean if r_mean != 0 else 0.0

        idx_averages[idx] = {
            'count': count,
            'l_mean': l_mean,
            'l_std': l_std,
            'l_std_err': l_std_err,
            'l_std_err_ratio': l_std_err_ratio,
            'r_mean': r_mean,
            'r_std': r_std,
            'r_std_err': r_std_err,
            'r_std_err_ratio': r_std_err_ratio
        }


if __name__ == '__main__':
    print_hi('PyCharm')

