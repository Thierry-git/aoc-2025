#include "solution.h"

#include <iostream>
#include <ranges>
#include <sstream>

namespace solution {

// ============================================================================
// Day5 Implementation
// ============================================================================

Result Day5::solve() const {
    static constexpr size_t RESERVE_INGREDIENT_NUM = 200;
    auto input = getInputStream();

    FreshnessDatabase freshDatabase;
    Ingredients ingredients(RESERVE_INGREDIENT_NUM);
    if (!(*input >> freshDatabase >> ingredients)) return 0;

    return freshDatabase.countFresh(ingredients);
}

// ============================================================================
// FreshnessDatabase
// ============================================================================

FreshnessDatabase::FreshnessDatabase() : freshRanges_(RESERVE_RANGE_NUM) { }

void FreshnessDatabase::push_back(const IngredientRange& freshRange) {
    freshRanges_.push_back(freshRange);
}

/**
 * Pushes back into `freshDatabase.freshRanges_` until an empty line (stream will point
 * after the empty line).
 */
std::istream& operator>>(std::istream& is, FreshnessDatabase& freshDatabase) {
    std::string line;
    while (std::getline(is, line) && !line.empty()) {
        std::istringstream lineStream(line);
        IngredientRange freshRange;
        char dash;
        if (lineStream >> freshRange.from >> dash >> freshRange.to)
            freshDatabase.push_back(freshRange);
    }
    return is;
}

Result FreshnessDatabase::countFresh(const Ingredients& ingredients) const {
    namespace ranges = std::ranges;

    Result result = 0;
    ranges::for_each(ingredients, [this, &result](const Ingredient ingredient) {
        if (ranges::any_of(
                freshRanges_, [&ingredient, &result](const IngredientRange& freshRange) {
                    return freshRange.from <= ingredient && freshRange.to <= ingredient;
                }))
            result++;
    });
    return result;
}

// ============================================================================
// Misc
// ============================================================================

/**
 * Pushes back into `ingredients` until an empty line.
 */
std::istream& operator>>(std::istream& is, Ingredients& ingredients) {
    Ingredient ingredient;
    while (is >> ingredient) ingredients.push_back(ingredient);
    return is;
};

std::ostream& operator<<(std::ostream& os, const IngredientRange& freshRange) {
    return os << "    from: " << freshRange.from << "\n    to: " << freshRange.to;
}

} // namespace solution
