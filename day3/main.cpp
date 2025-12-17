#include "solution.h"

#include <iostream>
#include <memory>
#include <optional>

using Part1Solution = solution::Day3Part1<solution::SingleThreadStrategy>;
using Part2Solution = solution::Day3Part2<solution::SingleThreadStrategy>;

static constexpr const char* INPUT_FILE = "input/input.txt";

enum class Part { PART1, PART2 };

std::optional<Part> parsePart(const std::string& arg) {
    if (arg == "part1" || arg == "1") return Part::PART1;
    if (arg == "part2" || arg == "2") return Part::PART2;
    return std::nullopt;
}

void printUsage(const char* progName) {
    std::cerr << "Usage: " << progName << " <part1|part2>" << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    const auto part = parsePart(argv[1]);
    if (!part) {
        std::cerr << "[ERROR] Invalid part: " << argv[1] << std::endl;
        printUsage(argv[0]);
        return EXIT_FAILURE;
    }

    std::unique_ptr<solution::Day3> solver;
    switch (*part) {
    case Part::PART1:
        solver = std::make_unique<Part1Solution>(INPUT_FILE);
        break;
    case Part::PART2:
        solver = std::make_unique<Part2Solution>(INPUT_FILE);
        break;
    }

    try {
        const auto result = solver->solve();
        std::cout << "[RESULT] " << result << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
