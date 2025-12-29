#pragma once

#include "../common/aoc.h"

#include <istream>
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

std::istream& operator>>(std::istream& is, Operation& op);

static constexpr int PROBLEM_LENGTH = 4;
struct Problem {
public:
    Problem() = default;
    std::array<Operand, PROBLEM_LENGTH> operands;
    Operation op;

    static bool isSentinel(const Problem& problem) { return problem.isSentinel_; }
    friend std::ostream& operator<<(std::ostream& os, const Problem& problem);

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
    std::array<Problem, PROBLEM_BUFSIZE>::const_iterator front_ = problems_.cbegin();
    std::array<Problem, PROBLEM_BUFSIZE>::iterator back_ = problems_.begin();
    std::counting_semaphore<PROBLEM_BUFSIZE> empty_ { PROBLEM_BUFSIZE };
    std::counting_semaphore<PROBLEM_BUFSIZE> full_ { 0 };
    mutable std::mutex mtx_;

    Problem retrieveFront();
    void affectBack(const Problem& problem);
};

class ResultMonitor {
public:
    ResultMonitor() = default;

    void add(const Result qty);
    Result get() const;

private:
    Result result_ = 0;
    mutable std::mutex mtx_;
};

static constexpr int NUM_PRODUCERS = 2;
static constexpr int NUM_CONSUMERS = 2;

class InputView {
public:
    InputView(const int fd, const size_t length);
    ~InputView();

    std::string_view getLine(const size_t index) const;

private:
    const size_t length_;
    const std::string_view input_;

    std::array<std::string_view, PROBLEM_LENGTH + 1> lines_;
};

template <typename T> class LineTokenizer {
public:
    LineTokenizer(const InputView& input, const size_t lineIndex);

    bool operator>>(T& out);

private:
    static constexpr int STRIDE = NUM_PRODUCERS;

    const std::string_view line_;
    size_t pos_;

    void skipStride();
    bool skipWhitespace();
    std::string currentEntry() const;
};

class Parser {
public:
    Parser(const InputView& input, const int colOffset);

    bool operator>>(Problem& problem);

private:
    template <size_t... Is>
    Parser(const InputView& input, const int colOffset, std::index_sequence<Is...>);

    std::array<LineTokenizer<Operand>, PROBLEM_LENGTH> operandTokenizers_;
    LineTokenizer<Operation> opTokenizer_;

    void init(int colOffset);
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
