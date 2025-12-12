#include "solution.h"

#include <iostream>

unsigned expected(int n, int part) {
    switch (n) {
    case 1:
        switch (part) {
        case 1:
            return 3;
        case 2:
            return 6;
        default:
            return -1;
        }
    case 2:
        switch (part) {
        case 1:
            return 4;
        case 2:
            return 23;
        default:
            return -1;
        }
    default:
        return -1;
    }
}

void test(int n) {
    static constexpr char TEST_INPUT_ROOT[] = "input/test";
    static constexpr int INIT_VALUE = 50;
    static constexpr int MODULUS = 100;

    const std::string LOG_PREFIX = "[TEST " + std::to_string(n) + "] ";

    const std::string testFilePath
        = std::string(TEST_INPUT_ROOT) + std::to_string(n) + ".txt";

    for (int part = 1; part <= 2; part++) {
        unsigned result;
        if (part == 1) {
            solution::Day1Part1 solver(testFilePath, INIT_VALUE, MODULUS);
            result = solver.solve();
        } else {
            solution::Day1Part2 solver(testFilePath, INIT_VALUE, MODULUS);
            result = solver.solve();
        }

        const bool success = result == expected(n, part);

        std::cout << LOG_PREFIX << "Part " << part << " result: " << result << std::endl;
        std::cout << LOG_PREFIX << (success ? "PASS" : "FAIL ");
        if (!success) std::cout << "(expected " << expected(n, part) << ")";
        std::cout << std::endl << std::endl;
    }
}

int main() {
    test(1);
    test(2);
    return 0;
}
