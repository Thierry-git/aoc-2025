#pragma once

// ============================================================================
// Template Implementations
// ============================================================================

namespace solution {

template <typename T>
    requires std::derived_from<T, Day3>
Joltage Day3Part1<T>::getJoltage(const Bank& bank) const {
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

    const char concat[3] = { first, second, '\0' };
    const Joltage joltage = atoi(concat);

    return joltage;
}

template <typename T>
    requires std::derived_from<T, Day3>
Joltage Day3Part2<T>::getJoltage(const Bank& bank) const {
    (void)bank;
    return 0;
}

} // namespace solution
