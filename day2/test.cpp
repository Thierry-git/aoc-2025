#include "solution.h"

#include <algorithm>
#include <iostream>
#include <sstream>

namespace solution {

template <typename SolverType> class TestDecorator : public SolverType {
public:
    TestDecorator(const std::string& inputFile) : SolverType(inputFile) {
        parseExpectedValue();
        _logPrefix = "[" + inputFile + "]";
        std::transform(
            _logPrefix.begin(), _logPrefix.end(), _logPrefix.begin(), ::toupper);
    }

    long solve() const {
        const long result = SolverType::solve();
        verifyResult(result);
        return result;
    }

    long getExpected() const { return _expected; }

protected:
    std::unique_ptr<std::istream> getInputStream() const override {
        std::unique_ptr<std::ifstream> input
            = std::make_unique<std::ifstream>(this->_inputFile);
        if (!input || !*input) throw std::runtime_error("Input file not found");

        // Skip header lines
        std::string line;
        std::getline(*input, line); // Skip "EXPECTED"
        std::getline(*input, line); // Skip expected value (already parsed)
        std::getline(*input, line); // Skip "DATA"

        return input;
    }

private:
    std::string _logPrefix = "[TEST]";

    void parseExpectedValue() {
        std::ifstream file(this->_inputFile);
        if (!file) throw std::runtime_error("Cannot open test file");

        std::string line;
        std::getline(file, line);
        std::getline(file, line);

        _expected = std::stol(line);
    }

    void verifyResult(long result) const {
        if (result == _expected) {
            std::cout << _logPrefix << " ✓ PASSED" << std::endl;
        } else {
            std::cout << _logPrefix << " ✗ FAILED: Expected " << _expected << ", got "
                      << result << std::endl;
        }
    }

    long _expected = -1;
};

using Day2Part1Test = TestDecorator<Day2Part1>;
using Day2Part2Test = TestDecorator<Day2Part2>;

}

int main() {
    static constexpr int NUM_TESTS_PART1 = 6;
    static constexpr int NUM_TESTS_PART2 = 2;

    for (int i = 1; i <= NUM_TESTS_PART1; i++) {
        const std::string testName = "input/part1-test" + std::to_string(i) + ".txt";
        const solution::Day2Part1Test test(testName);
        test.solve();
    }

    for (int i = 1; i <= NUM_TESTS_PART2; i++) {
        const std::string testName = "input/part2-test" + std::to_string(i) + ".txt";
        const solution::Day2Part2Test test(testName);
        test.solve();
    }

    return EXIT_SUCCESS;
}
