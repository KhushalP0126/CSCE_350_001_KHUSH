#include <array>
#include <cerrno>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

namespace {
constexpr const char *kInputDir = "inputs";
constexpr int kRunsPerSize = 25;
constexpr std::array<int, 3> kSizes = {10, 100, 1000};

bool ensureDirectory(const std::string &path) {
    if (mkdir(path.c_str(), 0755) == 0 || errno == EEXIST) {
        return true;
    }
    std::perror(("mkdir " + path).c_str());
    return false;
}

std::string buildFilename(int size, int run) {
    return std::string(kInputDir) + "/input_" + std::to_string(size) + "_run" + std::to_string(run) +
           ".txt";
}
} // namespace

int main() {
    if (!ensureDirectory(kInputDir)) {
        return 1;
    }

    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(-100, 100);

    for (int size : kSizes) {
        for (int run = 1; run <= kRunsPerSize; ++run) {
            const std::string filename = buildFilename(size, run);
            std::ofstream out(filename);
            if (!out) {
                std::cerr << "Error: Unable to open '" << filename << "' for writing." << std::endl;
                return 1;
            }

            for (int i = 0; i < size; ++i) {
                out << distribution(generator);
                if (i + 1 < size) {
                    out << ' ';
                }
            }
            out << '\n';
        }
    }

    std::cout << "Generated integer input files in '" << kInputDir << "' for sizes ";
    for (std::size_t i = 0; i < kSizes.size(); ++i) {
        std::cout << kSizes[i];
        if (i + 1 < kSizes.size()) {
            std::cout << ", ";
        }
    }
    std::cout << " (" << kRunsPerSize << " runs each)." << std::endl;

    return 0;
}
