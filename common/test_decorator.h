#pragma once

#include "solver.h"

#include <algorithm>
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
 * @tparam SolverType The concrete solver class to wrap (must be Solver<T> for some T)
 */
template <typename SolverType> class TestDecorator : public Solver<bool> {
    using ResultType = decltype(std::declval<SolverType>().solve());

public:
    explicit TestDecorator(const std::string& inputFile) :
    Solver<bool>(inputFile), _solver(inputFile), _logPrefix(makeLogPrefix(inputFile)) {
        parseExpectedValue();
    }

    /**
     * @brief Solve and verify against expected value.
     * @return true if the result matches expected, false otherwise
     */
    bool solve() const override {
        const auto result = _solver.solve();
        return verifyResult(result);
    }

    auto getExpected() const { return _expected; }

protected:
    /**
     * @brief Get input stream, skipping the test file header.
     *
     * Override for different header parsing behavior.
     *
     * @return Input stream positioned after the DATA line
     */
    std::unique_ptr<std::istream> getInputStream() const override {
        auto input = std::make_unique<std::ifstream>(this->_inputFile);
        if (!input || !*input) {
            throw std::runtime_error("Cannot open test file: " + this->_inputFile);
        }

        std::string line;
        std::getline(*input, line); // EXPECTED
        std::getline(*input, line); // (expected value)
        std::getline(*input, line); // DATA

        return input;
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
    SolverType _solver;
    std::string _logPrefix;
    ResultType _expected {};

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
};

} // namespace aoc
