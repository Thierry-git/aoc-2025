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
     * Wonky 'unmathematical' operators to determine if operations 'passed' zero
     * zero.
     */

    int distanceToZeroMovingLeft();
    int distanceToZeroMovingRight();

    bool operator==(const AdditiveIntMod& other) const;
    bool operator==(const int other) const;

private:
    int _value;
    int _modulus;

    static int addValuesMod(const int a, const int b, const int modulus) {
        return ((a + b) % modulus + modulus) % modulus;
    }
};