#pragma once

#include "../common/aoc.h"

#include <mutex>
#include <semaphore>

namespace solution {

// ============================================================================
// Day 6 Solution
// ============================================================================

using Result = long;

using Operand = int;
enum class Operation {
    Times = '*',
    Plus = '+',
};

static constexpr int PROBLEM_LENGTH = 4;
struct Problem {
    Problem() = delete;
    constexpr Problem() : operands {}, op(Operation::Times), isSentinel(false) { }

    std::array<Operand, PROBLEM_LENGTH> operands;
    Operation op;

private:
    bool isSentinel = false;

    friend class ProblemsMonitor;

    constexpr Problem() : operands({}), op(Operation::Times), isSentinel(true) { }
};

class ProblemsMonitor {
public:
    ProblemsMonitor() = default;

    void push_back(const Problem& problem);
    Problem pop_front();

    void pushSentinel() { push_back(SENTINEL); }

private:
    static constexpr int PROBLEM_BUFSIZE = 16;
    std::array<Problem, PROBLEM_BUFSIZE> problems_;
    std::counting_semaphore<PROBLEM_BUFSIZE> empty_ { PROBLEM_BUFSIZE };
    std::counting_semaphore<PROBLEM_BUFSIZE> full_ { 0 };
    std::mutex mtx_;

    static constexpr Problem SENTINEL = {};
};

class ResultMonitor {
public:
    ResultMonitor() = default;

    void add(Result qty);
    Result get();

private:
    Result result_ = 0;
    std::mutex mtx_;
};

/**
 * @brief Base class for Day 6.
 *
 * Contains shared logic between Part1 and Part2.
 */
class Day6 : public aoc::Solver<Result> {
public:
    explicit Day6(const std::string& inputFile) : Solver(inputFile) { }

    Result solve() const override;

protected:
    static constexpr int NUM_PRODUCERS = 2;
    static constexpr int NUM_CONSUMERS = 2;

    virtual void producer() const = 0;
    virtual void consumer() const = 0;
};

class Day6Part1 : public Day6 {
public:
    explicit Day6Part1(const std::string& inputFile) : Day6(inputFile) { }

protected:
    virtual void producer() const override;
    virtual void consumer() const override;
};

class Day6Part2 : public Day6 {
public:
    explicit Day6Part2(const std::string& inputFile) : Day6(inputFile) { }

protected:
    virtual void producer() const override;
    virtual void consumer() const override;
};

using Day6Part1Test = aoc::TestDecorator<Day6Part1>;
using Day6Part2Test = aoc::TestDecorator<Day6Part2>;

}; // namespace solution
