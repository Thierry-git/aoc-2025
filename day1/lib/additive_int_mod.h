#pragma once

#include <compare>

class AdditiveIntMod {
public:
    AdditiveIntMod(const int value, const int modulus);

    AdditiveIntMod& operator+=(const AdditiveIntMod& other);
    AdditiveIntMod& operator-=(const AdditiveIntMod& other);
    AdditiveIntMod operator+(const AdditiveIntMod& other) const;
    AdditiveIntMod operator-(const AdditiveIntMod& other) const;

    AdditiveIntMod& operator+=(const int other);
    AdditiveIntMod& operator-=(const int other);
    AdditiveIntMod operator+(const int other) const;
    AdditiveIntMod operator-(const int other) const;

    /**
     * Wonky 'unmathematical' operator to determine if operations 'passed' zero
     * zero.
     */
    std::strong_ordering operator<=>(const AdditiveIntMod& other) const;
    std::strong_ordering operator<=>(const int other) const;

    bool operator==(const AdditiveIntMod& other) const;
    bool operator==(const int other) const;

private:
    int _value;
    int _modulus;

    int addValuesMod(const int a, const int b) const { return (a + b) % _modulus; }
};