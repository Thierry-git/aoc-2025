#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 4 Solution
// ============================================================================

/**
 * @brief Base class for Day 4.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day4 : public aoc::Solver<long> {
public:
    explicit Day4(const std::string& inputFile) : Solver(inputFile) { }

    long solve() const override;

protected:
    // TODO: Add shared helper methods here

    /**
     * @brief Process a single item from the input.
     * Override in Part1/Part2 with different logic.
     */
    // virtual long processItem(...) const = 0;
};

class Day4Part1 : public Day4 {
public:
    explicit Day4Part1(const std::string& inputFile) : Day4(inputFile) { }

protected:
    // TODO: Override methods as needed
};

class Day4Part2 : public Day4 {
public:
    explicit Day4Part2(const std::string& inputFile) : Day4(inputFile) { }

protected:
    // TODO: Override methods as needed
};

using Day4Part1Test = aoc::TestDecorator<Day4Part1>;
using Day4Part2Test = aoc::TestDecorator<Day4Part2>;

}; // namespace solution
