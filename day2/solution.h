#pragma once

#include <fstream>
#include <iostream>
#include <memory>
#include <vector>

namespace solution {

struct Range {
    long from;
    int lenFrom;
    long to;
    int lenTo;
};

struct Pattern {
    int length;
    int reps;

    Pattern(int l, int r) : length(l), reps(r) { }
};

std::istream& operator>>(std::istream& is, solution::Range& range);

class Day2 {
public:
    Day2(const std::string& inputFile) : _inputFile(inputFile) { };
    virtual ~Day2() = default;

    long solve() const;

protected:
    virtual std::vector<Pattern> contributingPatterns(const Range& range) const = 0;
    virtual long contributionFrom(const Pattern& pattern, const Range& range) const;

    long computeInvalidSum(const Range& range) const;

    static long quotientCeil(long numerator, long denominator);
    static long quotientFloor(long numerator, long denominator);
    static long multiplierOf(const Pattern& pattern);

    virtual std::unique_ptr<std::istream> getInputStream() const;
    const std::string _inputFile;

    static constexpr int BASE = 10;
};

class Day2Part1 : public Day2 {
public:
    Day2Part1(const std::string& inputFile) : Day2(inputFile) { }

protected:
    std::vector<Pattern> contributingPatterns(const Range& range) const override;
};

class Day2Part2 : public Day2 {
public:
    Day2Part2(const std::string& inputFile) : Day2(inputFile) { };

protected:
    std::vector<Pattern> contributingPatterns(const Range& range) const override;
    long contributionFrom(const Pattern& pattern, const Range& range) const override;

private:
    static std::vector<int> divisors(const int n);
    static int mobiusFunction(int n);
};
};