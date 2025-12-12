#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 3 Solution
// ============================================================================

/**
 * @brief Base class for Day 3.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day3 : public aoc::Solver<long> {
public:
    explicit Day3(const std::string& inputFile) : Solver(inputFile) { }

    long solve() const override;

protected:
    // TODO: Add shared helper methods here

    /**
     * @brief Process a single item from the input.
     * Override in Part1/Part2 with different logic.
     */
    // virtual long processItem(...) const = 0;
};

class Day3Part1 : public Day3 {
public:
    explicit Day3Part1(const std::string& inputFile) : Day3(inputFile) { }

protected:
    // TODO: Override methods as needed
};

class Day3Part2 : public Day3 {
public:
    explicit Day3Part2(const std::string& inputFile) : Day3(inputFile) { }

protected:
    // TODO: Override methods as needed
};

using Day3Part1Test = aoc::TestDecorator<Day3Part1>;
using Day3Part2Test = aoc::TestDecorator<Day3Part2>;

}; // namespace solution
