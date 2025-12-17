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
    static const ssize_t nonTailBank = bank.size() - SEQUENCE_LENGTH;
    if (nonTailBank < 0) return 0;

    std::deque<char> improvements;
    std::deque<char> tail(bank.cbegin() + nonTailBank, bank.cend());
    std::deque<char>::const_iterator bad = tail.cbegin();
    /**
     * @returns true if a bad tail item was popped, false otherwise
     */
    const auto popBad = [&tail, &bad]() {
        const std::deque<char>::const_iterator end = tail.cend();
        while (bad + 1 < end && *bad >= *(bad + 1)) bad++;
        const bool foundBad = bad + 1 < end;
        if (foundBad) {
            bad = tail.erase(bad);
            bad--;
            if (bad < tail.cbegin()) bad = tail.cbegin();
        }
        return foundBad;
    };

    const Bank::const_reverse_iterator end = bank.crend();
    for (auto next = bank.crbegin() + SEQUENCE_LENGTH; next < end; next++) {
        if (!improvements.empty() && *next < improvements.front()) continue;
        improvements.push_front(*next);
        const auto lastDragged = improvements.cbegin() + SEQUENCE_LENGTH - tail.size();
        if (!tail.empty()) {
            const char top = tail.front();
            if (top >= *lastDragged) improvements.insert(lastDragged, top);

            if (!popBad()) {
                improvements.append_range(tail);
                tail.clear();
            };
        }
    }

    static const unsigned CONCAT_LENGTH = SEQUENCE_LENGTH + 1;
    char concat[CONCAT_LENGTH];
    concat[SEQUENCE_LENGTH] = '\0';

    const unsigned headSize = SEQUENCE_LENGTH - tail.size();
    for (unsigned i = 0; i < headSize; i++, improvements.pop_front())
        concat[i] = improvements.front();
    for (unsigned i = headSize; i < SEQUENCE_LENGTH; i++, tail.pop_front())
        concat[i] = tail.front();

    return atol(concat);
}

} // namespace solution
