#pragma once

#include "../common/aoc.h"

namespace solution {

// ============================================================================
// Day X Solution
// ============================================================================

using Result = long;

/**
 * @brief Base class for Day X.
 *
 * Contains shared logic between Part1 and Part2.
 */
class DayX : public aoc::Solver<Result> {
public:
    explicit DayX(const std::string& inputFile) : Solver(inputFile) { }

    Result solve() const override;

protected:
    // TODO: Add shared helper methods here

    /**
     * @brief Process a single item from the input.
     * Override in Part1/Part2 with different logic.
     */
    // virtual long processItem(...) const = 0;
};

class DayXPart1 : public DayX {
public:
    explicit DayXPart1(const std::string& inputFile) : DayX(inputFile) { }

protected:
    // TODO: Override methods as needed
};

class DayXPart2 : public DayX {
public:
    explicit DayXPart2(const std::string& inputFile) : DayX(inputFile) { }

protected:
    // TODO: Override methods as needed
};

using DayXPart1Test = aoc::TestDecorator<DayXPart1>;
using DayXPart2Test = aoc::TestDecorator<DayXPart2>;

}; // namespace solution
