#include "solution.h"

#include <iostream>

using namespace aoc;

int main() {
    static constexpr const char* INPUT_DIR = "input";

    std::cout << "===== Day 4 =====" << std::endl;

    auto part1Tests = discoverTests(INPUT_DIR, R"(part1-test\d+\.txt)");
    runTests<solution::Day4Part1Test>(part1Tests, "Part 1");

    auto part2Tests = discoverTests(INPUT_DIR, R"(part2-test\d+\.txt)");
    runTests<solution::Day4Part2Test>(part2Tests, "Part 2");

    return EXIT_SUCCESS;
}
