#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace aoc {

/**
 * @brief Abstract base class for Advent of Code solutions.
 *
 * Provides common infrastructure for input handling and solution structure.
 * Day classes should derive from this to implement each day's solution.
 *
 * @tparam ResultType The type of the solution result (default: long)
 */
template <typename ResultType = long> class Solver {
public:
    explicit Solver(const std::string& inputFile) : _inputFile(inputFile) { }
    virtual ~Solver() = default;

    /**
     * @brief Solve the puzzle and return the result.
     * @return The solution value
     */
    virtual ResultType solve() const = 0;

    const std::string& inputFile() const { return _inputFile; }

protected:
    /**
     * @brief Get an input stream for reading the puzzle input.
     *
     * Override method for custom input handling (e.g., for tests).
     *
     * @return Unique pointer to an input stream
     * @throws std::runtime_error if the file cannot be opened
     */
    virtual std::unique_ptr<std::istream> getInputStream() const {
        auto input = std::make_unique<std::ifstream>(_inputFile);
        if (!input || !*input) {
            throw std::runtime_error("Cannot open input file: " + _inputFile);
        }
        return input;
    }

    const std::string _inputFile;
};

} // namespace aoc
