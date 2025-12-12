#include "solution.h"

#include <iostream>
#include <sstream>

namespace solution {

template <typename SolverType> class TestDecorator : public SolverType {
public:
    TestDecorator(const std::string& inputFile) : SolverType(inputFile) {
        parseExpectedValue();
    }

    long solve() override {
        const long result = SolverType::solve();
        verifyResult(result);
        return result;
    }

    long getExpected() const { return _expected; }

protected:
    std::unique_ptr<std::istream> getInputStream() override {
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
            std::cout << "✓ Test PASSED: " << result << std::endl;
        } else {
            std::cout << "✗ Test FAILED: Expected " << _expected << ", got " << result
                      << std::endl;
        }
    }

    long _expected = -1;
};

using Day2Part1Test = TestDecorator<Day2Part1>;
using Day2Part2Test = TestDecorator<Day2Part2>;

}

int main() {
    solution::Day2Part1Test test("input/test1.txt");
    test.solve();

    return EXIT_SUCCESS;
}
