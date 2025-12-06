import csv
import math
import statistics
from collections import defaultdict

import matplotlib.pyplot as plt

RAW_TIMES_FILE = "Pandala_Khushal_executionTime.txt"
OUTPUT_IMAGE = "Pandala_Khushal_plotAverageExecutionTime.jpg"


def read_timings(filename):
    data = defaultdict(list)
    with open(filename, "r", encoding="utf-8") as file:
        reader = csv.DictReader(file)
        for row in reader:
            try:
                size = int(row["input_size"])
                time_ns = float(row["time_ns"])
            except (KeyError, ValueError):
                continue
            data[size].append(time_ns)
    return data


def _scale_curve(x_values, y_values):
    # Solve y ≈ a * x for a, where x is each theoretical point (like n log n).
    top = sum(x * y for x, y in zip(x_values, y_values))
    bottom = sum(x * x for x in x_values)
    return (top / bottom) if bottom else 0.0


def plot_averages(data):
    sizes = sorted(data.keys())
    averages = [statistics.mean(data[size]) for size in sizes]

    plt.figure(figsize=(8, 5))
    plt.plot(sizes, averages, marker="o", linestyle="-", color="blue", label="Measured Average")

    if sizes and sizes[0] > 1:
        n_log_n = [n * math.log2(n) for n in sizes]
        theta_scale = _scale_curve(n_log_n, averages)
        big_theta_curve = [theta_scale * value for value in n_log_n]

        plt.plot(sizes, big_theta_curve, linestyle="--", color="green", label="Big Theta (n log n)")
        plt.fill_between(sizes, big_theta_curve, color="green", alpha=0.08)

    plt.title("QuickSort Average Execution Time vs Input Size")
    plt.xlabel("Input Size")
    plt.ylabel("Average Time (nanoseconds)")
    plt.grid(True, linestyle="--", linewidth=0.5)
    plt.legend()
    plt.tight_layout()
    plt.savefig(OUTPUT_IMAGE)


def main():
    try:
        data = read_timings(RAW_TIMES_FILE)
    except FileNotFoundError:
        print(f"Error: '{RAW_TIMES_FILE}' not found. Run the experiments first.")
        return

    if not data:
        print("Error: Raw timing file is empty or malformed.")
        return

    plot_averages(data)

    print(f"Plot saved to '{OUTPUT_IMAGE}'.")


if __name__ == "__main__":
    main()
