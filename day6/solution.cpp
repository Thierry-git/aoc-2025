#include "solution.h"

#include <functional>
#include <istream>
#include <ranges>
#include <sys/stat.h>
#include <thread>
#include <vector>

namespace solution {

// ============================================================================
// Day6 Implementation
// ============================================================================

Result Day6::solve() const {
    const int fd = getFileDescriptor();
    struct stat sb;
    if (fstat(fd, &sb) < 0) {
        close(fd);
        throw new std::system_error;
    }

    InputView input(fd, sb.st_size);

    ProblemsMonitor problems;
    ResultMonitor results;

    std::vector<std::jthread> producers, consumers;
    producers.reserve(NUM_PRODUCERS);
    consumers.reserve(NUM_CONSUMERS);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producers.emplace_back([this, &input, &problems, i]() {
            ProducerArgs args { makeParser(input, i), problems };
            producer(args);
        });
    }

    for (int i = 0; i < NUM_CONSUMERS; i++) {
        consumers.emplace_back([this, &problems, &results]() {
            ConsumerArgs args { problems, results };
            consumer(args);
        });
    }

    producers.clear(); /* Producers are done */

    /* Signal consumers to exit */
    for (int i = 0; i < NUM_CONSUMERS; i++) problems.pushSentinel();

    consumers.clear();

    return results.get();
}

std::unique_ptr<Parser> Day6Part1::makeParser(
    const InputView& input, const unsigned colOffset) const {
    return std::make_unique<HorizontalParser>(input, colOffset);
}

void Day6Part1::producer(ProducerArgs& args) const {
    Problem problem;
    while (*args.parser >> problem) {
        args.problems.push_back(problem);
    }
}

void Day6Part1::consumer(ConsumerArgs& args) const {
    namespace ranges = std::ranges;

    static const auto sum
        = [](const Result acc, const Operand operand) { return acc + operand; };

    static const auto prod
        = [](const Result acc, const Operand operand) { return acc * operand; };

    Result result = 0;
    for (;;) {
        const Problem problem = args.problems.pop_front();
        if (Problem::isSentinel(problem)) break;

        switch (problem.op) {
        case Operation::Plus:
            result += ranges::fold_left(problem.operands, (Result)0, sum);
            break;
        case Operation::Times:
            result += ranges::fold_left(problem.operands, (Result)1, prod);
            break;
        default:
            result += 0;
            break;
        }
    }

    args.result.add(result);
}

// TODO
std::unique_ptr<Parser> Day6Part2::makeParser(
    const InputView& input, const unsigned colOffset) const {
    (void)input;
    (void)colOffset;
    return nullptr;
}

void Day6Part2::producer(ProducerArgs& args) const {
    (void)args;
    return;
}

void Day6Part2::consumer(ConsumerArgs& args) const {
    (void)args;
    return;
}

// ============================================================================
// Monitors
// ============================================================================

void ProblemsMonitor::push_back(const Problem& problem) {
    empty_.acquire();
    std::lock_guard<std::mutex> lock(mtx_);
    affectBack(problem);
    full_.release();
}

Problem ProblemsMonitor::pop_front() {
    full_.acquire();
    std::lock_guard<std::mutex> lock(mtx_);
    const Problem problem = retrieveFront();
    empty_.release();
    return problem;
}

void ProblemsMonitor::affectBack(const Problem& problem) {
    static const auto begin = problems_.begin();
    static const auto end = problems_.end();
    *back_++ = problem;
    if (back_ == end) back_ = begin;
}

Problem ProblemsMonitor::retrieveFront() {
    static const auto begin = problems_.cbegin();
    static const auto end = problems_.cend();
    const Problem& problem = *front_++;
    if (front_ == end) front_ = begin;
    return problem;
}

void ResultMonitor::add(const Result qty) {
    std::lock_guard<std::mutex> lock(mtx_);
    result_ += qty;
}

Result ResultMonitor::get() const {
    std::lock_guard<std::mutex> lock(mtx_);
    return result_;
}

// ============================================================================
// Input and parsing
// ============================================================================

InputView::InputView(const int fd, const size_t length) :
length_(length), input_((char*)mmap(0, length, PROT_READ, MAP_SHARED, fd, 0)) {
    size_t start = 0;
    for (int i = 0; i <= PROBLEM_LENGTH; i++) {
        size_t end = input_.find('\n', start);
        lines_[i] = input_.substr(start, end - start + 1);
        start = end + 1;
    }
}

InputView::~InputView() {
    munmap((void*)input_.data(), length_);
}

std::string_view InputView::getLine(const size_t index) const {
    return index <= PROBLEM_LENGTH ? lines_[index] : "\n";
}

HorizontalParser::HorizontalParser(const InputView& input, const unsigned colOffset) :
HorizontalParser(input, colOffset, std::make_index_sequence<PROBLEM_LENGTH> {}) { }

template <size_t... Is>
HorizontalParser::HorizontalParser(
    const InputView& input, const unsigned colOffset, std::index_sequence<Is...>) :
operandTokenizers_ { { LineTokenizer<Operand>(input, Is, colOffset)... } },
opTokenizer_({ input, PROBLEM_LENGTH, colOffset }) { }

unsigned moduloProducers(const int colOffset) {
    int mod = colOffset % (int)NUM_PRODUCERS;
    if (mod < 0) mod += NUM_PRODUCERS;
    return mod;
}

bool HorizontalParser::operator>>(Problem& problem) {
    bool success = true;
    for (int i = 0; i < PROBLEM_LENGTH; i++)
        success &= operandTokenizers_[i] >> problem.operands[i];
    success &= opTokenizer_ >> problem.op;
    return success;
}

template <typename T>
LineTokenizer<T>::LineTokenizer(const InputView& input, const size_t lineIndex,
    const unsigned colOffset) : line_(input.getLine(lineIndex)) {
    skipWhitespace();
    skipEntries(colOffset);
}

template <typename T>
bool LineTokenizer<T>::operator>>(T& out)
    requires StreamExtractable<T>
{
    std::stringstream ss;
    std::string tmp(this->currentEntry());
    ss << tmp;

    if (!(ss >> out)) return false;

    this->skipStride();
    return true;
}

template <typename T> void LineTokenizer<T>::skipStride() {
    skipEntries(STRIDE);
}

template <typename T> void LineTokenizer<T>::skipEntries(const unsigned n) {
    for (unsigned i = 0; i < n; i++) {
        const size_t newPos = line_.find_first_of(" \n", pos_);
        if (newPos == std::string::npos) return;
        pos_ = newPos;
        if (!this->skipWhitespace()) return;
    }
}

template <typename T> bool LineTokenizer<T>::skipWhitespace() {
    const size_t newPos = line_.find_first_not_of(" ", pos_);
    if (newPos == std::string::npos) return false;
    pos_ = newPos;
    return true;
}

template <typename T> std::string LineTokenizer<T>::currentEntry() const {
    const size_t end = line_.find_first_of(" \n", pos_);
    if (end == std::string::npos) return "";
    return std::string(line_.substr(pos_, end - pos_ + 1));
}

std::istream& operator>>(std::istream& is, Operation& op) {
    char c;
    is >> c;
    switch (c) {
    case '+':
        op = Operation::Plus;
        break;

    case '*':
    default:
        op = Operation::Times;
        break;
    }
    return is;
}

// ============================================================================
// Misc
// ============================================================================

std::ostream& operator<<(std::ostream& os, const Problem& problem) {
    for (int i = 0; i < PROBLEM_LENGTH - 1; i++)
        os << problem.operands[i] << ' ' << problem.op << ' ';
    return os << problem.operands[PROBLEM_LENGTH - 1];
}

std::ostream& operator<<(std::ostream& os, const Operation& op) {
    switch (op) {
    case Operation::Plus:
        os << '+';
        break;

    case Operation::Times:
    default:
        os << '*';
        break;
    }
    return os;
}

} // namespace solution
