#include "solution.h"

#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

namespace solution {

// ============================================================================
// Threading Strategies
// ============================================================================

std::shared_ptr<const ThreadingStrategy> SingleThreadStrategy::get() {
    static auto instance = std::make_shared<const SingleThreadStrategy>();
    return instance;
}

Joltage SingleThreadStrategy::operator()(std::istream& is, JoltageCalculator calc) const {
    Joltage total = 0;
    Bank bank;
    while (std::getline(is, bank)) total += calc(bank);
    return total;
}

std::shared_ptr<const ThreadingStrategy> MultiThreadStrategy::get() {
    static auto instance = std::make_shared<const MultiThreadStrategy>();
    return instance;
}

Joltage MultiThreadStrategy::operator()(std::istream& is, JoltageCalculator calc) const {
    std::vector<Bank> banks;
    Bank bank;
    while (std::getline(is, bank)) banks.push_back(bank);
    if (banks.empty()) return 0;

    const size_t banksSize = banks.size();
    const unsigned numThreads
        = std::min(std::thread::hardware_concurrency(), static_cast<unsigned>(banksSize));

    std::vector<std::jthread> threads;
    threads.reserve(numThreads);

    Joltage total = 0;
    std::mutex resultMutex;

    size_t lower = 0, upper = 0;
    for (unsigned i = 0; i < numThreads; ++i, lower = upper, upper += banksSize) {
        threads.emplace_back(
            [&banks, &total, &resultMutex, &calc, lower, upper, numThreads, banksSize]() {
                const size_t start = lower / numThreads;
                const size_t end = upper / numThreads;

                Joltage localSum = 0;
                for (size_t j = start; j < end; ++j) localSum += calc(banks[j]);

                const std::lock_guard<std::mutex> lock(resultMutex);
                total += localSum;
            });
    }

    threads.clear();

    return total;
}

// ============================================================================
// Day3 Implementation
// ============================================================================

Joltage Day3::solve() const {
    auto input = getInputStream();
    return (*strategy_)(*input, [this](const Bank& b) { return getJoltage(b); });
}

} // namespace solution
