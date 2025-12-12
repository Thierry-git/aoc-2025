#pragma once

#include <fstream>
#include <memory>
#include <string>

namespace solution {
enum class Direction {
    LEFT,
    RIGHT,
};

struct Rotation {
    Direction direction;
    unsigned value;
};

std::istream& operator>>(std::istream& is, Rotation& rotation);

template <unsigned N> class AdditiveIntMod {
public:
    AdditiveIntMod(int value = 0) : _value(value % N) { }

    AdditiveIntMod<N>& operator+=(const AdditiveIntMod<N>& other) {
        _value = addValuesModN(_value, other._value);
        return *this;
    }

    AdditiveIntMod<N>& operator-=(const AdditiveIntMod<N>& other) {
        _value = addValuesModN(_value, -other._value);
        return *this;
    }

    AdditiveIntMod<N> operator+(const AdditiveIntMod<N>& other) const {
        AdditiveIntMod<N> result = *this;
        result += other;
        return result;
    }

    AdditiveIntMod<N> operator-(const AdditiveIntMod<N>& other) const {
        AdditiveIntMod<N> result = *this;
        result -= other;
        return result;
    }

    /**
     * Wonky "unmathematical" operator to determine if operations "passed" or "ended at"
     * zero.
     */
    std::strong_ordering operator<=>(const AdditiveIntMod<N>& other) const {
        return _value <=> other._value;
    }

private:
    int _value;

    int addValuesModN(const int a, const int b) const { return (a + b) % N; }
};

class Day1 {
public:
    Day1(const std::string& inputPath, const int initValue, const int modulus) :
    _inputPath(inputPath), _initValue(initValue % modulus), _modulus(modulus) { }

    unsigned solvePart1() const;
    unsigned solvePart2() const;

private:
    const std::string _inputPath;
    const int _initValue;
    const int _modulus;

    std::unique_ptr<std::istream> _getInput() const {
        std::unique_ptr<std::istream> input = std::make_unique<std::ifstream>(_inputPath);
        if (!input || !*input)
            throw std::runtime_error("Failed to open input file: " + _inputPath);
        return input;
    }
};
};
