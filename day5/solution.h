#pragma once

#include "../common/aoc.h"

#include <iostream>
#include <vector>

namespace solution {

// ============================================================================
// Day 5 Solution
// ============================================================================

using Result = int;
using Ingredient = long;
using Ingredients = std::vector<Ingredient>;
template <typename T> struct Range {
    T from;
    T to;
};
using IngredientRange = Range<Ingredient>;

class FreshnessDatabase {
public:
    FreshnessDatabase();

    friend std::istream& operator>>(std::istream& is, FreshnessDatabase& freshDatabase);

    Result countFresh(const Ingredients& ingredients) const;

private:
    static constexpr size_t RESERVE_RANGE_NUM = 100;
    std::vector<IngredientRange> freshRanges_;

    void push_back(const IngredientRange& freshRange);
};

std::istream& operator>>(std::istream& is, Ingredients& ingredients);

std::ostream& operator<<(std::ostream& os, const IngredientRange& freshRange);

/**
 * @brief Base class for Day 5.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day5 : public aoc::Solver<Result> {
public:
    explicit Day5(const std::string& inputFile) : Solver(inputFile) { }

    Result solve() const override;

protected:
};

class Day5Part1 : public Day5 {
public:
    explicit Day5Part1(const std::string& inputFile) : Day5(inputFile) { }

protected:
};

class Day5Part2 : public Day5 {
public:
    explicit Day5Part2(const std::string& inputFile) : Day5(inputFile) { }

protected:
};

using Day5Part1Test = aoc::TestDecorator<Day5Part1>;
using Day5Part2Test = aoc::TestDecorator<Day5Part2>;
}; // namespace solution
