#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day 3 Solution
// ============================================================================

using Battery = char;
using Bank = std::basic_string<Battery>;
using Joltage = int;

/**
 * @brief Base class for Day 3.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day3 : public aoc::Solver<Joltage> {
public:
    explicit Day3(const std::string& inputFile) : Solver(inputFile) { }

    Joltage solve() const override;

protected:
    virtual Joltage getJoltage(const Bank& bank) const = 0;
};

class Day3Part1 : public Day3 {
public:
    explicit Day3Part1(const std::string& inputFile) : Day3(inputFile) { }

protected:
    Joltage getJoltage(const Bank& bank) const;
};

class Day3Part2 : public Day3 {
public:
    explicit Day3Part2(const std::string& inputFile) : Day3(inputFile) { }

protected:
    Joltage getJoltage(const Bank& bank) const;
};

using Day3Part1Test = aoc::TestDecorator<Day3Part1>;
using Day3Part2Test = aoc::TestDecorator<Day3Part2>;

}; // namespace solution
