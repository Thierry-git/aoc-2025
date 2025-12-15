#include "solution.h"

using namespace aoc;

class Day3MultiThread : public solution::Day3 {
public:
    Day3MultiThread(const std::string& inputFile) :
    Day3(inputFile, solution::MultiThreadStrategy::get()) { }
};

int main() {
    static constexpr const char* INPUT_DIR = "input";

    auto part1Tests = discoverTests(INPUT_DIR, R"(part1-test\d+\.txt)");
    runTests<solution::Day3Part1Test>(part1Tests, "Part 1");
    runTests<TestDecorator<solution::Day3Part1<Day3MultiThread>>>(
        part1Tests, "Part 1 (Multi-Threaded)");

    auto part2Tests = discoverTests(INPUT_DIR, R"(part2-test\d+\.txt)");
    runTests<solution::Day3Part2Test>(part2Tests, "Part 2");
    runTests<TestDecorator<solution::Day3Part2<Day3MultiThread>>>(
        part2Tests, "Part 2 (Multi-Threaded)");

    return EXIT_SUCCESS;
}
