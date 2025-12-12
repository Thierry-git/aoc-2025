#pragma once

#include <fstream>
#include <iostream>

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
    Day2() = default;
    virtual ~Day2() = default;

    virtual void solve() = 0;
};

class Day2Part1 : public Day2 {
public:
    Day2Part1() = default;
    ~Day2Part1() override = default;

    void solve() override { };
};

class Day2Part2 : public Day2 {
public:
    Day2Part2() = default;
    ~Day2Part2() override = default;
    void solve() override { };
};
};