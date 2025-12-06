#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace {
constexpr const char *kSingleRunTimeFile = "Pandala_Khushal_executionTime_singleRun.txt";

// Chooses the pivot index using the median-of-three rule (left, middle, right).
int medianOfThreeIndex(const std::vector<int> &arr, int left, int right) {
    const int mid = left + (right - left) / 2;
    const int a = arr[left];
    const int b = arr[mid];
    const int c = arr[right];

    if ((a <= b && b <= c) || (c <= b && b <= a)) {
        return mid;
    }
    if ((b <= a && a <= c) || (c <= a && a <= b)) {
        return left;
    }
    return right;
}

// Hoare partitioning around a median-of-three pivot.
int partition(std::vector<int> &arr, int left, int right) {
    const int pivotIndex = medianOfThreeIndex(arr, left, right);
    std::swap(arr[left], arr[pivotIndex]);
    const int pivot = arr[left];

    int i = left - 1;
    int j = right + 1;
    while (true) {
        do {
            ++i;
        } while (arr[i] < pivot);
        do {
            --j;
        } while (arr[j] > pivot);
        if (i >= j) {
            return j;
        }
        std::swap(arr[i], arr[j]);
    }
}

// Recursive QuickSort driver on the closed interval [left, right].
void quickSort(std::vector<int> &arr, int left, int right) {
    if (left >= right) {
        return;
    }
    int pivotPos = partition(arr, left, right);
    quickSort(arr, left, pivotPos);
    quickSort(arr, pivotPos + 1, right);
}

bool readInput(const std::string &filename, std::vector<int> &values) {
    std::ifstream input(filename);
    if (!input) {
        return false;
    }
    int value = 0;
    values.clear();
    while (input >> value) {
        values.push_back(value);
    }
    return true;
}

bool writeOutput(const std::string &filename, const std::vector<int> &values) {
    std::ofstream output(filename);
    if (!output) {
        return false;
    }
    for (std::size_t i = 0; i < values.size(); ++i) {
        output << values[i];
        if (i + 1 < values.size()) {
            output << ' ';
        }
    }
    output << '\n';
    return true;
}

void recordExecutionTime(long long microseconds) {
    std::ofstream timeFile(kSingleRunTimeFile);
    if (timeFile) {
        timeFile << microseconds << '\n';
    }
}
} // namespace

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::string inputFilename = argv[1];
    std::string outputFilename = argv[2];
    std::vector<int> values;

    if (!readInput(inputFilename, values)) {
        std::cerr << "Error: Unable to open input file '" << inputFilename << "'." << std::endl;
        return 1;
    }

    auto start = std::chrono::high_resolution_clock::now();
    quickSort(values, 0, static_cast<int>(values.size()) - 1);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    if (duration == 0) {
        duration = 1; // ensure very small runs still register a measurable duration
    }
    recordExecutionTime(duration);
    std::cout << "TIME_NS=" << duration << std::endl;

    if (!writeOutput(outputFilename, values)) {
        std::cerr << "Error: Unable to open output file '" << outputFilename << "'." << std::endl;
        return 1;
    }

    return 0;
}
