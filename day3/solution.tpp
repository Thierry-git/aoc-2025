#pragma once

// ============================================================================
// Template Implementations
// ============================================================================

namespace solution {

template <typename Strat>
    requires std::derived_from<Strat, ThreadingStrategy>
Joltage Day3Part1<Strat>::getJoltage(const Bank& bank) const {
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

template <typename Strat>
    requires std::derived_from<Strat, ThreadingStrategy>
Joltage Day3Part2<Strat>::getJoltage(const Bank& bank) const {
    const size_t n = bank.size();
    if (n < SEQUENCE_LENGTH) return 0;

    std::stack<char> sequence;
    size_t toDrop = n - SEQUENCE_LENGTH;

    for (const char c : bank) {
        for (; !sequence.empty() && toDrop > 0 && sequence.top() < c; toDrop--)
            sequence.pop();
        sequence.push(c);
    }

    while (sequence.size() > SEQUENCE_LENGTH) sequence.pop();

    char concat[SEQUENCE_LENGTH + 1];
    concat[SEQUENCE_LENGTH] = '\0';
    for (unsigned i = SEQUENCE_LENGTH; 0 < i; i--, sequence.pop())
        concat[i - 1] = sequence.top();

    return atol(concat);
}

} // namespace solution
