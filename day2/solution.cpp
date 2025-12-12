#include "solution.h"

#include <cmath>
#include <iostream>

std::unique_ptr<std::istream> solution::Day2::getInputStream() {
    std::unique_ptr<std::ifstream> input = std::make_unique<std::ifstream>(_inputFile);
    if (!input || !*input) throw std::runtime_error("Input file not found");

    return input;
}

std::istream& solution::operator>>(std::istream& is, solution::Range& range) {
    char dash;
    if (!(is >> range.from >> dash >> range.to)) return is;

    range.lenFrom = std::to_string(range.from).length();
    range.lenTo = std::to_string(range.to).length();

    /* Remove trailing comma if it exists */
    if (is.peek() == ',') is.get();

    return is;
}

long solution::Day2Part1::solve() {
    std::unique_ptr<std::istream> input = getInputStream();

    long sum = 0;
    Range range;
    while (*input >> range) sum += computeInvalidSum(range);

    return sum;
}

long solution::Day2Part1::computeInvalidSum(const Range& range) {
    const int minRepeatedDigits = quotientCeil(range.lenFrom, 2);
    const int maxRepeatedDigits = quotientFloor(range.lenTo, 2);

    long contribution = 0;
    for (int k = minRepeatedDigits; k <= maxRepeatedDigits; k++)
        contribution += contributionFrom(k, range);

    return contribution;
}

long solution::Day2Part1::contributionFrom(int numRepeatedDigits, const Range& range) {
    const long minimum = std::pow(10, numRepeatedDigits - 1);
    const long maximum = 10 * minimum - 1;
    const long zeroSandwich = maximum + 2;

    long lower = quotientCeil(range.from, zeroSandwich);
    lower = lower >= minimum ? lower : 1;
    long upper = quotientFloor(range.to, zeroSandwich);
    upper = upper <= maximum ? upper : maximum;

    const long actualLower = lower * zeroSandwich;
    const long actualUpper = upper * zeroSandwich;

    return (actualUpper * (upper + 1) - (lower - 1) * actualLower) / 2;
}

long solution::Day2Part1::quotientCeil(long numerator, long denominator) {
    return (numerator + denominator - 1) / denominator;
}

long solution::Day2Part1::quotientFloor(long numerator, long denominator) {
    return numerator / denominator;
}
