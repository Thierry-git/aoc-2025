#pragma once

#include "../common/aoc.h"

#include <iostream>
#include <memory>
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

class PushBackStrategy {
public:
    virtual void operator()(std::vector<IngredientRange>& freshRanges,
        const IngredientRange& freshRange) const
        = 0;
};

class PushBackEverything : public PushBackStrategy {
public:
    virtual void operator()(std::vector<IngredientRange>& freshRanges,
        const IngredientRange& freshRange) const override;
};

class CombineOnPushBack : public PushBackStrategy {
public:
    virtual void operator()(std::vector<IngredientRange>& freshRanges,
        const IngredientRange& freshRange) const override;
};

class FreshnessDatabase {
public:
    FreshnessDatabase();
    virtual ~FreshnessDatabase() = default;

    friend std::istream& operator>>(std::istream& is, FreshnessDatabase& freshDatabase);

    Result countFresh(const Ingredients& ingredients) const;
    Result sumRangeLengths() const;

    void setPushBackStrategy(std::unique_ptr<PushBackStrategy> strategy);

private:
    static constexpr size_t RESERVE_RANGE_NUM = 100;
    std::vector<IngredientRange> freshRanges_;
    std::unique_ptr<PushBackStrategy> strategy_;

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

    virtual Result solve() const = 0;

protected:
    virtual std::unique_ptr<FreshnessDatabase> createDatabase() const = 0;
};

class Day5Part1 : public Day5 {
public:
    explicit Day5Part1(const std::string& inputFile) : Day5(inputFile) { }

    virtual Result solve() const override;

protected:
    virtual std::unique_ptr<FreshnessDatabase> createDatabase() const override;
};

class Day5Part2 : public Day5 {
public:
    explicit Day5Part2(const std::string& inputFile) : Day5(inputFile) { }

    virtual Result solve() const override;

protected:
    virtual std::unique_ptr<FreshnessDatabase> createDatabase() const override;
};

using Day5Part1Test = aoc::TestDecorator<Day5Part1>;
using Day5Part2Test = aoc::TestDecorator<Day5Part2>;
}; // namespace solution
