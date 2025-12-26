#include "solution.h"

#include <functional>
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
        throw std::system_error();
    }

    InputView input(fd, sb.st_size);

    ProblemsMonitor problems;
    ResultMonitor results;

    std::vector<std::jthread> producers, consumers;
    producers.reserve(NUM_PRODUCERS);
    consumers.reserve(NUM_CONSUMERS);

    for (int i = 0; i < NUM_PRODUCERS; i++) {
        producers.emplace_back([this, &input, &problems, i]() {
            ProducerArgs args { Parser(input, i), problems };
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

void Day6Part1::producer(ProducerArgs& args) const {
    Problem problem;
    while (args.parser >> problem) args.problems.push_back(problem);
}

void Day6Part1::consumer(ConsumerArgs& args) const {
    namespace ranges = std::ranges;

    static const auto sum
        = [](const Result acc, const Operand operand) { return acc + operand; };

    static const auto prod
        = [](const Result acc, const Operand operand) { return acc + operand; };

    for (;;) {
        const Problem problem = args.problems.pop_front();
        if (Problem::isSentinel(problem)) return;

        Result result;
        switch (problem.op) {
        case Operation::Plus:
            result = ranges::fold_left(problem.operands, (Result)0, sum);
            break;
        case Operation::Times:
            result = ranges::fold_left(problem.operands, (Result)0, prod);
            break;
        default:
            result = 0;
            break;
        }

        args.result.add(result);
    }
}

void Day6Part2::producer(ProducerArgs& args) const {
    return;
}

void Day6Part2::consumer(ConsumerArgs& args) const {
    return;
}

} // namespace solution
