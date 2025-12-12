#include "solution.h"

using namespace aoc;

int main() {
    static constexpr const char* INPUT_DIR = "input";

    auto part1Tests = discoverTests(INPUT_DIR, R"(part1-test\d+\.txt)");
    runTests<solution::DayXPart1Test>(part1Tests, "Part 1");

    auto part2Tests = discoverTests(INPUT_DIR, R"(part2-test\d+\.txt)");
    runTests<solution::DayXPart2Test>(part2Tests, "Part 2");

    return EXIT_SUCCESS;
}
