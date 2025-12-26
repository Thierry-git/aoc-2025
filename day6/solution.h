#pragma once

#include "../common/aoc.h"

#include <mutex>
#include <semaphore>
#include <sys/mman.h>

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
public:
    Problem() = default;
    std::array<Operand, PROBLEM_LENGTH> operands;
    Operation op;

    static bool isSentinel(const Problem& problem) { return problem.isSentinel_; }

private:
    bool isSentinel_ = false;

    friend class ProblemsMonitor;

    constexpr Problem(bool isSentinel) :
    operands({}), op(Operation::Times), isSentinel_(isSentinel) { }
};

class ProblemsMonitor {
public:
    ProblemsMonitor() = default;

    void push_back(const Problem& problem);
    Problem pop_front();

    void pushSentinel() { push_back(Problem(true)); }

private:
    static constexpr int PROBLEM_BUFSIZE = 16;
    std::array<Problem, PROBLEM_BUFSIZE> problems_ = {};
    std::counting_semaphore<PROBLEM_BUFSIZE> empty_ { PROBLEM_BUFSIZE };
    std::counting_semaphore<PROBLEM_BUFSIZE> full_ { 0 };
    std::mutex mtx_;
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

static constexpr int NUM_PRODUCERS = 2;
static constexpr int NUM_CONSUMERS = 2;

class InputView {
public:
    InputView(const int fd, size_t length) :
    length_(length), input_((char*)mmap(0, length, PROT_READ, MAP_SHARED, fd, 0)) { }
    ~InputView() { munmap((void*)input_.data(), length_); }

    const std::string_view& getView() const { return input_; }
    size_t getLineSize() const;

private:
    const size_t length_;
    const std::string_view input_;
};

class Parser {
public:
    Parser(const InputView& input, const int colOffset);

    bool operator>>(Problem& problem);

private:
    off_t offset_;
    std::stringstream streams_[5];

    void advanceToNext();
};

struct ProducerArgs {
    Parser parser;
    ProblemsMonitor& problems;
};

struct ConsumerArgs {
    ProblemsMonitor& problems;
    ResultMonitor& result;
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
    virtual void producer(ProducerArgs& args) const = 0;
    virtual void consumer(ConsumerArgs& args) const = 0;
};

class Day6Part1 : public Day6 {
public:
    explicit Day6Part1(const std::string& inputFile) : Day6(inputFile) { }

protected:
    virtual void producer(ProducerArgs& args) const override;
    virtual void consumer(ConsumerArgs& args) const override;
};

class Day6Part2 : public Day6 {
public:
    explicit Day6Part2(const std::string& inputFile) : Day6(inputFile) { }

protected:
    virtual void producer(ProducerArgs& args) const override;
    virtual void consumer(ConsumerArgs& args) const override;
};

using Day6Part1Test = aoc::TestDecorator<Day6Part1>;
using Day6Part2Test = aoc::TestDecorator<Day6Part2>;

}; // namespace solution
