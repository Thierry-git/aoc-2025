#pragma once

#include <filesystem>
#include <regex>
#include <string>
#include <vector>

namespace aoc {

/**
 * @brief Discover test files matching a pattern in a directory.
 * @param dir Directory to search
 * @param pattern Regex pattern to match (e.g., "part1-test\\d+\\.txt")
 * @return Sorted vector of matching file paths
 */
inline std::vector<std::string> discoverTests(
    const std::string& dir, const std::string& pattern) {

    std::vector<std::string> tests;
    std::regex re(pattern);

    if (!std::filesystem::exists(dir)) return tests;

    for (const auto& entry : std::filesystem::directory_iterator(dir)) {
        if (entry.is_regular_file()) {
            std::string filename = entry.path().filename().string();
            if (std::regex_match(filename, re)) {
                tests.push_back(entry.path().string());
            }
        }
    }

    std::sort(tests.begin(), tests.end());
    return tests;
}

/**
 * @brief Run a set of tests and report results.
 * @tparam TestType The test decorator type to instantiate
 * @param testFiles Vector of test file paths
 * @param partName Name for logging (e.g., "Part 1")
 */
template <typename TestType>
void runTests(const std::vector<std::string>& testFiles, const std::string& partName) {
    if (testFiles.empty()) {
        std::cout << "[" << partName << "] No test files found" << std::endl;
        return;
    }

    std::cout << "\n=== " << partName << " Tests ===" << std::endl;

    int passed = 0;
    int failed = 0;

    for (const auto& testFile : testFiles) {
        try {
            TestType test(testFile);
            if (test.test()) {
                passed++;
            } else {
                failed++;
            }
        } catch (const std::exception& e) {
            std::cerr << "[" << testFile << "] ERROR: " << e.what() << std::endl;
            failed++;
        }
    }

    std::cout << "[" << partName << "] " << passed << " passed, " << failed << " failed\n"
              << std::endl;
}

} // namespace aoc
