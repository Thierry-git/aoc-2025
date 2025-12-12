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

std::unique_ptr<std::istream>& operator>>(
    std::unique_ptr<std::istream>& is, solution::Range& range);

class Day2 {
public:
    Day2(const std::string& inputFile) : _inputFile(inputFile) { };
    virtual ~Day2() = default;

    virtual long solve() = 0;

protected:
    std::unique_ptr<std::istream> getInputStream();

private:
    const std::string _inputFile;
};

class Day2Part1 : public Day2 {
public:
    Day2Part1(const std::string& inputFile) : Day2(inputFile) { };
    long solve() override { };

private:
    static long computeInvalidSum(const Range& range);
    static long quotientCeil(long numerator, long denominator);
    static long quotientFloor(long numerator, long denominator);
    static long contributionFrom(int numRepeatedDigits, const Range& range);
};

class Day2Part2 : public Day2 {
public:
    Day2Part2(const std::string& inputFile) : Day2(inputFile) { };
    long solve() override { };
};
};