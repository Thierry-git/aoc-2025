#pragma once

#include <fstream>
#include <iostream>
#include <memory>

namespace solution {

struct Range {
    int from;
    int to;
};

std::istream& operator>>(std::istream& is, Range& range) {
    char dash;
    is >> range.from >> dash >> range.to;
    return is >> dash; /* Remove trailing comma if it exists */
}

class Day2 {
public:
    Day2(const std::string& inputFile) : _inputFile(inputFile) { };
    virtual ~Day2() = default;

    virtual void solve() = 0;

protected:
    std::unique_ptr<std::istream> getInputStream();

private:
    const std::string _inputFile;
};

class Day2Part1 : public Day2 {
public:
    Day2Part1(const std::string& inputFile) : Day2(inputFile) { };
    void solve() override { };
};

class Day2Part2 : public Day2 {
public:
    Day2Part2(const std::string& inputFile) : Day2(inputFile) { };
    void solve() override { };
};
};