#include "solution.h"

#include <iostream>
#include <memory>
#include <optional>
#include <string>

static constexpr char* INPUT_FILE = "input/input.txt";

enum class Part {
    PART1,
    PART2,
};

std::optional<Part> extractPart(std::string part) {
    if (part == "part1") {
        return Part::PART1;
    } else if (part == "part2") {
        return Part::PART2;
    } else {
        return std::nullopt;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "[ERROR] Specify 'part1' or 'part2'" << std::endl;
        return EXIT_FAILURE;
    }

    const std::optional<Part> part = extractPart(argv[1]);
    if (!part) {
        std::cout << "[ERROR] Specify 'part1' or 'part2'" << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<solution::Day2> solver;
    switch (*part) {
    case Part::PART1:
        solver = std::make_unique<solution::Day2Part1>(INPUT_FILE);
        break;
    case Part::PART2:
        solver = std::make_unique<solution::Day2Part2>(INPUT_FILE);
        break;
    }

    try {
        const long solution = solver->solve();
        std::cout << "[LOG] The summation of all bad IDs is " << solution << std::endl;
    } catch (std::runtime_error e) {
        std::cout << "[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}