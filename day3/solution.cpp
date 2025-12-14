#include "solution.h"

namespace solution {

// ============================================================================
// Day3 Implementation
// ============================================================================

int Day3::solve() const {
    auto input = getInputStream();

    int joltage = 0;

    Bank bank;
    while (std::getline(*input, bank)) joltage += getJoltage(bank);

    return joltage;
}

// ============================================================================
// Part-specific overrides
// ============================================================================

Joltage Day3Part1::getJoltage(const Bank& bank) const {
    if (bank.size() < 2) return 0;

    auto current = bank.cbegin();
    auto last = --bank.cend();

    Battery first = *current;
    Battery second = *last;

    ++current;
    for (; current < last; ++current) {
        const Battery& battery = *current;
        if (first < battery) {
            first = battery;
            second = *last;
        } else if (second < battery) {
            second = battery;
        }
    }

    const char concat[2] = { first, second };

    return atoi(concat);
}

Joltage Day3Part2::getJoltage(const Bank& bank) const {
    (void)bank;
    return 0;
}

} // namespace solution
