#include "solution.h"

#include <iostream>
#include <ranges>
#include <sstream>

namespace solution {

// ============================================================================
// Day5 Implementation
// ============================================================================

Result Day5Part1::solve() const {
    static constexpr size_t RESERVE_INGREDIENT_NUM = 200;
    auto input = getInputStream();

    std::unique_ptr<FreshnessDatabase> freshDatabase = createDatabase();
    Ingredients ingredients;
    ingredients.reserve(RESERVE_INGREDIENT_NUM);
    *input >> *freshDatabase >> ingredients;

    return freshDatabase->countFresh(ingredients);
}

std::unique_ptr<FreshnessDatabase> Day5Part1::createDatabase() const {
    std::unique_ptr<FreshnessDatabase> database = std::make_unique<FreshnessDatabase>();
    database->setPushBackStrategy(std::make_unique<PushBackEverything>());
    return database;
}

Result Day5Part2::solve() const {
    auto input = getInputStream();

    std::unique_ptr<FreshnessDatabase> freshDatabase = createDatabase();
    *input >> *freshDatabase;

    return freshDatabase->sumRangeLengths();
}

std::unique_ptr<FreshnessDatabase> Day5Part2::createDatabase() const {
    std::unique_ptr<FreshnessDatabase> database = std::make_unique<FreshnessDatabase>();
    database->setPushBackStrategy(std::make_unique<CombineOnPushBack>());
    return database;
}

// ============================================================================
// FreshnessDatabase
// ============================================================================

FreshnessDatabase::FreshnessDatabase() {
    freshRanges_.reserve(RESERVE_RANGE_NUM);
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
        const bool isFresh = ranges::any_of(
            freshRanges_, [&ingredient](const IngredientRange& freshRange) {
                return freshRange.from <= ingredient && ingredient <= freshRange.to;
            });
        if (isFresh) result++;
    });
    return result;
}

Result FreshnessDatabase::sumRangeLengths() const {
    namespace ranges = std::ranges;

    return ranges::fold_left(
        freshRanges_, 0, [](const Result acc, const IngredientRange& freshRange) {
            return acc + (freshRange.to - freshRange.from);
        });
}

void FreshnessDatabase::push_back(const IngredientRange& freshRange) {
    (*strategy_)(freshRanges_, freshRange);
}

void FreshnessDatabase::setPushBackStrategy(std::unique_ptr<PushBackStrategy> strategy) {
    strategy_ = std::move(strategy);
}

// ============================================================================
// PushBackStrategy
// ============================================================================

void PushBackEverything::operator()(
    std::vector<IngredientRange>& freshRanges, const IngredientRange& freshRange) const {
    freshRanges.push_back(freshRange);
}

void CombineOnPushBack::operator()(
    std::vector<IngredientRange>& freshRanges, const IngredientRange& freshRange) const {
    freshRanges.push_back(freshRange);
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
