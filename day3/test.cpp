#include "solution.h"

using namespace aoc;

int main() {
    static constexpr const char* INPUT_DIR = "input";

    auto part1Tests = discoverTests(INPUT_DIR, R"(part1-test\d+\.txt)");
    runTests<solution::Day3Part1Test>(part1Tests, "Part 1");
    runTests<solution::Day3Part1MultiThreadTest>(part1Tests, "Part 1 (Multi-Threaded)");

    auto part2Tests = discoverTests(INPUT_DIR, R"(part2-test\d+\.txt)");
    runTests<solution::Day3Part2Test>(part2Tests, "Part 2");
    runTests<solution::Day3Part2MultiThreadTest>(part2Tests, "Part 2 (Multi-Threaded)");

    return EXIT_SUCCESS;
}
