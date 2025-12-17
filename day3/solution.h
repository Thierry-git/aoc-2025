#pragma once

#include "../common/aoc.h"

#include <functional>
#include <istream>
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
 * @brief Used to memory map the input file and get related data.
 */
class InputManager {
public:
    class BankIterator {
    public:
        friend class InputManager;

        bool operator>>(Bank& bank);

    private:
        BankIterator(const std::string_view& data, const size_t& start, const size_t& end,
            const size_t& digitsPerLine) :
        data_(data), pos_(start), end_(end), digitsPerLine_(digitsPerLine) { };

        const std::string_view& data_;
        std::size_t pos_;

        const size_t end_;
        const size_t digitsPerLine_;
    };

    InputManager(const int fd);
    ~InputManager();

    unsigned numThreads() const { return numThreads_; }
    BankIterator banksOfNextThread();

private:
    char* mapped_;
    size_t fileSize_;
    size_t digitsPerLine_;

    unsigned nextThread_ = 0;
    unsigned numThreads_;
    size_t bytesPerThread_;

    std::string_view data_;
    mutable std::mutex mtx_;
};

/**
 * @brief Abstract base for threading strategies.
 *
 * Receives an input stream and a callback to compute joltage per bank.
 */
class ThreadingStrategy {
public:
    ThreadingStrategy(const aoc::Solver<Joltage>& solver) : solver_(&solver) { }
    virtual Joltage operator()(const JoltageCalculator& calc) const = 0;
    virtual ~ThreadingStrategy() = default;

protected:
    const aoc::Solver<Joltage>* solver_;
};

/**
 * @brief Single-threaded strategy: processes banks sequentially.
 */
class SingleThreadStrategy : public ThreadingStrategy {
public:
    SingleThreadStrategy(const aoc::Solver<Joltage>& solver) :
    ThreadingStrategy(solver) { }
    Joltage operator()(const JoltageCalculator& calc) const override;
};

/**
 * @brief Multi-threaded strategy: distributes banks across CPU cores.
 */
class MultiThreadStrategy : public ThreadingStrategy {
public:
    MultiThreadStrategy(const aoc::Solver<Joltage>& solver) :
    ThreadingStrategy(solver) { }
    Joltage operator()(const JoltageCalculator& calc) const override;

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
    explicit Day3(
        const std::string& inputFile, std::unique_ptr<ThreadingStrategy> strategy) :
    Solver(inputFile), strategy_(std::move(strategy)) { }

    Joltage solve() const override;

protected:
    virtual Joltage getJoltage(const Bank& bank) const = 0;

private:
    std::unique_ptr<const ThreadingStrategy> strategy_;
};

template <typename Strat>
    requires std::derived_from<Strat, ThreadingStrategy>
class Day3Part1 : public Day3 {
public:
    explicit Day3Part1(const std::string& inputFile) :
    Day3(inputFile, std::make_unique<Strat>(*this)) { }

protected:
    Joltage getJoltage(const Bank& bank) const;
};

template <typename Strat>
    requires std::derived_from<Strat, ThreadingStrategy>
class Day3Part2 : public Day3 {
public:
    static constexpr unsigned SEQUENCE_LENGTH = 12;

    explicit Day3Part2(const std::string& inputFile) :
    Day3(inputFile, std::make_unique<Strat>(*this)) { }

protected:
    Joltage getJoltage(const Bank& bank) const;
};

using Day3Part1Test = aoc::TestDecorator<Day3Part1<SingleThreadStrategy>>;
using Day3Part1MultiThreadTest = aoc::TestDecorator<Day3Part1<MultiThreadStrategy>>;
using Day3Part2Test = aoc::TestDecorator<Day3Part2<SingleThreadStrategy>>;
using Day3Part2MultiThreadTest = aoc::TestDecorator<Day3Part2<MultiThreadStrategy>>;

}; // namespace solution

#include "solution.tpp"
