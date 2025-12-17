#pragma once

#include "../common/aoc.h"

#include <functional>
#include <memory>
#include <mutex>

namespace solution {

// ============================================================================
// Day 3 Solution
// ============================================================================

using Battery = char;
using Bank = std::basic_string<Battery>;
using Joltage = int;
using JoltageCalculator = std::function<Joltage(const Bank&)>;

/**
 * @brief Abstract base for threading strategies.
 *
 * Receives an input stream and a callback to compute joltage per bank.
 */
class ThreadingStrategy {
public:
    virtual Joltage operator()(std::istream& is, JoltageCalculator calc) const = 0;
    virtual ~ThreadingStrategy() = default;
};

/**
 * @brief Single-threaded strategy: processes banks sequentially.
 */
class SingleThreadStrategy : public ThreadingStrategy {
public:
    static std::shared_ptr<const ThreadingStrategy> get();
    Joltage operator()(std::istream& is, JoltageCalculator calc) const override;
};

/**
 * @brief Multi-threaded strategy: distributes banks across CPU cores.
 */
class MultiThreadStrategy : public ThreadingStrategy {
public:
    static std::shared_ptr<const ThreadingStrategy> get();
    Joltage operator()(std::istream& is, JoltageCalculator calc) const override;

private:
    class JoltageMonitor {
    public:
        JoltageMonitor(const Joltage joltage) : joltage_(joltage) { }

        Joltage get() const;
        void incrementBy(const Joltage joltage);

    private:
        Joltage joltage_;
        std::mutex mtx;
    };
};

/**
 * @brief Base class for Day 3.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day3 : public aoc::Solver<Joltage> {
public:
    explicit Day3(const std::string& inputFile,
        std::shared_ptr<const ThreadingStrategy> strategy = SingleThreadStrategy::get()) :
    Solver(inputFile), strategy_(strategy) { }

    Joltage solve() const override;

protected:
    virtual Joltage getJoltage(const Bank& bank) const = 0;

private:
    std::shared_ptr<const ThreadingStrategy> strategy_;
};

template <typename T = Day3>
    requires std::derived_from<T, Day3>
class Day3Part1 : public T {
public:
    explicit Day3Part1(const std::string& inputFile) : T(inputFile) { }

protected:
    Joltage getJoltage(const Bank& bank) const;
};

template <typename T = Day3>
    requires std::derived_from<T, Day3>
class Day3Part2 : public T {
public:
    static constexpr unsigned SEQUENCE_LENGTH = 12;

    explicit Day3Part2(const std::string& inputFile) : T(inputFile) { }

protected:
    Joltage getJoltage(const Bank& bank) const;
};

using Day3Part1Test = aoc::TestDecorator<Day3Part1<>>;
using Day3Part2Test = aoc::TestDecorator<Day3Part2<>>;

}; // namespace solution

#include "solution.tpp"
