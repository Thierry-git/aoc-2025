#pragma once

#include "solver.h"

#include <unistd.h>

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <sstream>
#include <string>

namespace aoc {

/**
 * @brief Decorator that wraps a solver to add test verification.
 *
 * Test input files should have the format:
 *   EXPECTED
 *   <expected_value>
 *   DATA
 *   <puzzle input>
 *
 * The TestDecorator intercepts getInputStream() to skip the header lines,
 * parses the expected value, and verifies the result after solve().
 *
 * For file-based access (e.g., mmap), getDataFilePath() creates a temporary
 * file containing only the data portion.
 *
 * @tparam SolverType The concrete solver class to wrap (must be Solver<T> for some T)
 */
template <typename SolverType> class TestDecorator : private SolverType {
    using ResultType = decltype(std::declval<SolverType>().solve());

public:
    explicit TestDecorator(const std::string& inputFile) :
    SolverType(inputFile), _logPrefix(makeLogPrefix(inputFile)) {
        parseExpectedValue();
    }

    ~TestDecorator() { cleanupTempFile(); }

    // Disable copy (temp file ownership)
    TestDecorator(const TestDecorator&) = delete;
    TestDecorator& operator=(const TestDecorator&) = delete;

    // Enable move
    TestDecorator(TestDecorator&& other) noexcept :
    SolverType(std::move(other)), _logPrefix(std::move(other._logPrefix)),
    _expected(std::move(other._expected)), _tempDataFile(std::move(other._tempDataFile)) {
        other._tempDataFile.clear();
    }

    TestDecorator& operator=(TestDecorator&& other) noexcept {
        if (this != &other) {
            cleanupTempFile();
            SolverType::operator=(std::move(other));
            _logPrefix = std::move(other._logPrefix);
            _expected = std::move(other._expected);
            _tempDataFile = std::move(other._tempDataFile);
            other._tempDataFile.clear();
        }
        return *this;
    }

    /**
     * @brief Solve and verify against expected value.
     * @return true if the result matches expected, false otherwise
     */
    bool test() const {
        const auto result = SolverType::solve();
        return verifyResult(result);
    }

    auto getExpected() const { return _expected; }

protected:
    /**
     * @brief Get path to a file containing only the data portion.
     *
     * Creates a temporary file on first call, stripping the test header.
     * The temp file is cleaned up when the TestDecorator is destroyed.
     *
     * @return Path to temporary file with raw puzzle data
     */
    std::string getDataFilePath() const override {
        if (_tempDataFile.empty()) {
            _tempDataFile = createTempDataFile();
        }
        return _tempDataFile;
    }

    /**
     * @brief Verify the result against expected value.
     *
     * Override for custom verification behavior.
     * @return true if verification passed, false otherwise
     */
    template <typename T> bool verifyResult(const T& result) const {
        const bool success = result == _expected;
        if (success) {
            std::cout << _logPrefix << " ✓ PASSED" << std::endl;
        } else {
            std::cout << _logPrefix << " ✗ FAILED: Expected " << _expected << ", got "
                      << result << std::endl;
        }
        return success;
    }

    const std::string& logPrefix() const { return _logPrefix; }

private:
    std::string _logPrefix;
    ResultType _expected {};
    mutable std::string _tempDataFile;

    static std::string makeLogPrefix(const std::string& inputFile) {
        std::string prefix = "[" + inputFile + "]";
        std::transform(prefix.begin(), prefix.end(), prefix.begin(), ::toupper);
        return prefix;
    }

    void parseExpectedValue() {
        std::ifstream file(this->_inputFile);
        if (!file) {
            throw std::runtime_error("Cannot open test file: " + this->_inputFile);
        }

        std::string line;
        std::getline(file, line); // EXPECTED
        std::getline(file, line); // (expected value)

        std::istringstream iss(line);
        iss >> _expected;
    }

    /**
     * @brief Create a temporary file containing only the data portion.
     * @return Path to the created temporary file
     */
    std::string createTempDataFile() const {
        std::string tempTemplate = "/tmp/aoc-test-XXXXXX";
        std::vector<char> tempPath(tempTemplate.begin(), tempTemplate.end());
        tempPath.push_back('\0');

        int fd = mkstemp(tempPath.data());
        if (fd < 0) {
            throw std::runtime_error(
                "Cannot create temp file: " + std::string(std::strerror(errno)));
        }

        // Open source and skip header
        std::ifstream src(this->_inputFile, std::ios::binary);
        if (!src) {
            close(fd);
            unlink(tempPath.data());
            throw std::runtime_error("Cannot open test file: " + this->_inputFile);
        }

        std::string line;
        std::getline(src, line); // EXPECTED
        std::getline(src, line); // (expected value)
        std::getline(src, line); // DATA

        std::ostringstream buffer;
        buffer << src.rdbuf();
        std::string data = buffer.str();

        if (write(fd, data.data(), data.size()) < 0) {
            close(fd);
            unlink(tempPath.data());
            throw std::runtime_error("Cannot write to temp file");
        }

        close(fd);
        return std::string(tempPath.data());
    }

    void cleanupTempFile() {
        if (!_tempDataFile.empty()) {
            unlink(_tempDataFile.c_str());
            _tempDataFile.clear();
        }
    }
};

} // namespace aoc
