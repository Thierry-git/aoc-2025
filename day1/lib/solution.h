#pragma once

#include "additive_int_mod.h"

#include <compare>
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
    int value;
};

std::istream& operator>>(std::istream& is, Rotation& rotation);

class Day1 {
public:
    Day1(const std::string& inputPath, const int initValue, const int modulus) :
    _inputPath(inputPath), _initValue(initValue % modulus), _modulus(modulus) { }

    unsigned solve() const;

private:
    const std::string _inputPath;
    const int _initValue;

    std::unique_ptr<std::istream> _getInput() const {
        std::unique_ptr<std::istream> input = std::make_unique<std::ifstream>(_inputPath);
        if (!input || !*input)
            throw std::runtime_error("Failed to open input file: " + _inputPath);
        return input;
    }

protected:
    /**
     * @returns the quantity to increment password by when rotating
     *
     * @note Must rotate `current` appropriately as a side-effect
     */
    virtual unsigned rotate(AdditiveIntMod& current, const Rotation& rotation) const = 0;

    const int _modulus;
};

class Day1Part1 : public Day1 {
public:
    Day1Part1(const std::string& inputPath, const int initValue, const int modulus) :
    Day1(inputPath, initValue, modulus) { }

protected:
    unsigned rotate(AdditiveIntMod& current, const Rotation& rotation) const override;
};

class Day1Part2 : public Day1 {
public:
    Day1Part2(const std::string& inputPath, const int initValue, const int modulus) :
    Day1(inputPath, initValue, modulus) { }

protected:
    unsigned rotate(AdditiveIntMod& current, const Rotation& rotation) const override;
};
};
