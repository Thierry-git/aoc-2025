#pragma once

#include <fstream>
#include <iostream>
#include <memory>

namespace solution {

struct Range {
    long from;
    int lenFrom;
    long to;
    int lenTo;
};

std::istream& operator>>(std::istream& is, solution::Range& range);

class Day2 {
public:
    Day2(const std::string& inputFile) : _inputFile(inputFile) { };
    virtual ~Day2() = default;

    long solve() const;

protected:
    virtual long contributionFrom(int numRepeatedDigits, const Range& range) const = 0;
    long computeInvalidSum(const Range& range) const;

    static long quotientCeil(long numerator, long denominator);
    static long quotientFloor(long numerator, long denominator);

    virtual std::unique_ptr<std::istream> getInputStream() const;
    const std::string _inputFile;
};

class Day2Part1 : public Day2 {
public:
    Day2Part1(const std::string& inputFile) : Day2(inputFile) { };

protected:
    long contributionFrom(int numRepeatedDigits, const Range& range) const override;
};

class Day2Part2 : public Day2 {
public:
    Day2Part2(const std::string& inputFile) : Day2(inputFile) { };

protected:
    long contributionFrom(int numRepeatedDigits, const Range& range) const override {
        return 0;
    };
};
};