#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 6 Solution
// ============================================================================

using Result = long;

/**
 * @brief Base class for Day 6.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day6 : public aoc::Solver<Result> {
public:
    explicit Day6(const std::string& inputFile) : Solver(inputFile) { }

    Result solve() const override;

protected:
    // TODO: Add shared helper methods here

    /**
     * @brief Process a single item from the input.
     * Override in Part1/Part2 with different logic.
     */
    // virtual long processItem(...) const = 0;
};

class Day6Part1 : public Day6 {
public:
    explicit Day6Part1(const std::string& inputFile) : Day6(inputFile) { }

protected:
    // TODO: Override methods as needed
};

class Day6Part2 : public Day6 {
public:
    explicit Day6Part2(const std::string& inputFile) : Day6(inputFile) { }

protected:
    // TODO: Override methods as needed
};

using Day6Part1Test = aoc::TestDecorator<Day6Part1>;
using Day6Part2Test = aoc::TestDecorator<Day6Part2>;

}; // namespace solution
