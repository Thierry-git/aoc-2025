#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 5 Solution
// ============================================================================

/**
 * @brief Base class for Day 5.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day5 : public aoc::Solver<long> {
public:
    explicit Day5(const std::string& inputFile) : Solver(inputFile) { }

    long solve() const override;

protected:
    // TODO: Add shared helper methods here

    /**
     * @brief Process a single item from the input.
     * Override in Part1/Part2 with different logic.
     */
    // virtual long processItem(...) const = 0;
};

class Day5Part1 : public Day5 {
public:
    explicit Day5Part1(const std::string& inputFile) : Day5(inputFile) { }

protected:
    // TODO: Override methods as needed
};

class Day5Part2 : public Day5 {
public:
    explicit Day5Part2(const std::string& inputFile) : Day5(inputFile) { }

protected:
    // TODO: Override methods as needed
};

using Day5Part1Test = aoc::TestDecorator<Day5Part1>;
using Day5Part2Test = aoc::TestDecorator<Day5Part2>;

}; // namespace solution
