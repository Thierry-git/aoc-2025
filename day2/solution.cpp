#include "solution.h"

#include <iostream>

std::unique_ptr<std::istream> solution::Day2::getInputStream() {
    std::unique_ptr<std::ifstream> input = std::make_unique<std::ifstream>(_inputFile);
    if (!input || !*input) throw std::runtime_error("Input file not found");
    return input;
}

std::unique_ptr<std::istream>& operator>>(
    std::unique_ptr<std::istream>& is, solution::Range& range) {
    char dash;
    *is >> range.strFrom >> dash >> range.strTo;
    *is >> dash; /* Remove trailing comma if it exists */

    range.from = atol(range.strFrom.c_str());
    range.to = atol(range.strTo.c_str());

    return is;
}

long solution::Day2Part1::solve() {
    std::unique_ptr<std::istream> input = getInputStream();

    long value = 0;
    Range range;
    while (input >> range) value += computeInvalidSum(range);

    return value;
}

long solution::Day2Part1::computeInvalidSum(const Range& range) {
    const long length = range.to - range.from;

    long value = 0;
}
