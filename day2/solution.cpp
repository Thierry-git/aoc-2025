#include "solution.h"

#include <cmath>
#include <iostream>

std::unique_ptr<std::istream> solution::Day2::getInputStream() const {

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

long solution::Day2::solve() const {

    std::unique_ptr<std::istream> input = getInputStream();

    long sum = 0;
    Range range;
    while (*input >> range) {
        long contribution = computeInvalidSum(range);
        sum += contribution;
    }

    return sum;
}

long solution::Day2::computeInvalidSum(const Range& range) const {

    long contribution = 0;
    for (const Pattern& pattern : contributingPatterns(range))
        contribution += contributionFrom(pattern, range);

    return contribution;
}

long solution::Day2::contributionFrom(const Pattern& pattern, const Range& range) const {

    const long minimum = std::pow(BASE, pattern.length - 1);
    const long maximum = BASE * minimum - 1;
    const long multiplier = multiplierOf(pattern);

    long lower = quotientCeil(range.from, multiplier);
    long upper = quotientFloor(range.to, multiplier);

    if (lower < minimum) lower = minimum;
    if (upper > maximum) upper = maximum;

    if (lower > upper) return 0;

    const long count = upper - lower + 1;
    const long sum = (lower + upper) * count / 2;

    return multiplier * sum;
}

std::vector<solution::Pattern> solution::Day2Part1::contributingPatterns(
    const Range& range) const {

    std::vector<Pattern> patterns;
    for (int totalDigits = range.lenFrom; totalDigits <= range.lenTo; totalDigits++) {
        if ((totalDigits % 2) != 0) continue;
        patterns.emplace_back(totalDigits / 2, 2);
    }
    return patterns;
}

std::vector<solution::Pattern> solution::Day2Part2::contributingPatterns(
    const Range& range) const {

    std::vector<Pattern> patterns;
    for (int totalDigits = range.lenFrom; totalDigits <= range.lenTo; totalDigits++) {
        for (int patternLen : divisors(totalDigits)) {
            const int reps = totalDigits / patternLen;
            if (reps < 2) continue;
            patterns.emplace_back(patternLen, reps);
        }
    }
    return patterns;
}

std::vector<int> solution::Day2Part2::divisors(const int n) {

    std::vector<int> divs;
    for (int d = 1; d <= n / 2; d++)
        if ((n % d) == 0) divs.push_back(d);

    return divs;
}

int solution::Day2Part2::mobiusFunction(int n) {
    if (n == 1) return 1;

    int primeFactors = 0;
    int temp = n;

    for (int p = 2; p * p <= temp; p++) {
        if (temp % p == 0) {
            primeFactors++;
            temp /= p;
            if (temp % p == 0) return 0;
        }
    }

    if (temp > 1) primeFactors++;

    return (primeFactors % 2 == 0) ? 1 : -1;
}

long solution::Day2Part2::contributionFrom(
    const Pattern& pattern, const Range& range) const {
    long baseContribution = Day2::contributionFrom(pattern, range);
    int mu = mobiusFunction(pattern.reps);
    return -mu * baseContribution;
}

long solution::Day2::multiplierOf(const Pattern& pattern) {

    long multiplier = 0;
    for (int i = 0; i < pattern.reps; i++)
        multiplier += std::pow(BASE, i * pattern.length);
    return multiplier;
}

long solution::Day2::quotientCeil(long numerator, long denominator) {

    return (numerator + denominator - 1) / denominator;
}

long solution::Day2::quotientFloor(long numerator, long denominator) {

    return numerator / denominator;
}
