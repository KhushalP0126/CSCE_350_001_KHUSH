#include <array>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

namespace {
constexpr const char *kQuickSortExecutable = "./Pandala_Khushal_QuickSort";
constexpr const char *kSingleRunTimeFile = "Pandala_Khushal_executionTime_singleRun.txt";
constexpr const char *kExecutionTimeLog = "Pandala_Khushal_executionTime.txt";
constexpr const char *kInputDir = "inputs";
constexpr const char *kOutputDir = "outputs";
constexpr int kRunsPerSize = 25;
constexpr std::array<int, 3> kSizes = {10, 100, 1000};

bool ensureDirectory(const std::string &path) {
    if (mkdir(path.c_str(), 0755) == 0 || errno == EEXIST) {
        return true;
    }
    std::perror(("mkdir " + path).c_str());
    return false;
}

std::string buildInputFilename(int size, int run) {
    return std::string(kInputDir) + "/input_" + std::to_string(size) + "_run" + std::to_string(run) +
           ".txt";
}

std::string buildOutputFilename(int size, int run) {
    return std::string(kOutputDir) + "/output_" + std::to_string(size) + "_run" + std::to_string(run) +
           ".txt";
}

long long readSingleRunTime() {
    std::ifstream timeFile(kSingleRunTimeFile);
    long long nanoseconds = 0;
    if (!timeFile || !(timeFile >> nanoseconds)) {
        throw std::runtime_error("Failed to read execution time from " + std::string(kSingleRunTimeFile));
    }
    return nanoseconds;
}
} // namespace

int main() {
    if (!ensureDirectory(kInputDir) || !ensureDirectory(kOutputDir)) {
        return 1;
    }

    std::ofstream timeLog(kExecutionTimeLog);
    if (!timeLog) {
        std::cerr << "Error: Unable to open '" << kExecutionTimeLog << "' for writing." << std::endl;
        return 1;
    }
    timeLog << "input_file,output_file,input_size,time_ns\n";

    for (int size : kSizes) {
        for (int run = 1; run <= kRunsPerSize; ++run) {
            const std::string inputFile = buildInputFilename(size, run);
            const std::string outputFile = buildOutputFilename(size, run);
            const std::string command = std::string(kQuickSortExecutable) + " " + inputFile + " " + outputFile;

            if (std::system(command.c_str()) != 0) {
                std::cerr << "Error: Command failed ('" << command << "')." << std::endl;
                return 1;
            }

            long long nanoseconds = 0;
            try {
                nanoseconds = readSingleRunTime();
            } catch (const std::exception &ex) {
                std::cerr << "Error: " << ex.what() << std::endl;
                return 1;
            }

            timeLog << inputFile << ',' << outputFile << ',' << size << ',' << nanoseconds << '\n';
        }
    }

    std::cout << "Finished running experiments. Raw timings stored in '" << kExecutionTimeLog
              << "'. Run the Python plotting script to visualize the averages." << std::endl;
    return 0;
}
