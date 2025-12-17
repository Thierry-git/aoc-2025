#pragma once

#include <fcntl.h>
#include <unistd.h>

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
 * Input access methods (getInputStream, getFileDescriptor) are built on top
 * of getDataFilePath(), which is the single virtual override point for tests.
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
     * @brief Get a path to a file containing raw puzzle data.
     *
     * Overridden for custom input handling (e.g., for tests).
     * All other input access methods are built on top of this.
     *
     * @return Path to a file containing only puzzle data
     */
    virtual std::string getDataFilePath() const { return _inputFile; }

    /**
     * @brief Get an input stream for reading the puzzle input.
     * @return Unique pointer to an input stream
     * @throws std::runtime_error if the file cannot be opened
     */
    std::unique_ptr<std::istream> getInputStream() const {
        auto path = getDataFilePath();
        auto input = std::make_unique<std::ifstream>(path);
        if (!input || !*input) {
            throw std::runtime_error("Cannot open input file: " + path);
        }
        return input;
    }

    /**
     * @brief Get a file descriptor for the puzzle input.
     *
     * Useful for mmap or other low-level file operations.
     * Caller is responsible for closing the file descriptor.
     *
     * @return Open file descriptor (O_RDONLY)
     * @throws std::runtime_error if the file cannot be opened
     */
    int getFileDescriptor() const {
        auto path = getDataFilePath();
        int fd = open(path.c_str(), O_RDONLY);
        if (fd < 0) {
            throw std::runtime_error("Cannot open input file: " + path);
        }
        return fd;
    }

    const std::string _inputFile;
};

} // namespace aoc
