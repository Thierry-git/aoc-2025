#include "additive_int_mod.h"

AdditiveIntMod::AdditiveIntMod(const int value, const int modulus) :
_value(value % modulus), _modulus(modulus) { }

AdditiveIntMod& AdditiveIntMod::operator+=(const AdditiveIntMod& other) {
    _value = addValuesMod(_value, other._value);
    return *this;
}

AdditiveIntMod& AdditiveIntMod::operator-=(const AdditiveIntMod& other) {
    _value = addValuesMod(_value, -other._value);
    return *this;
}

AdditiveIntMod AdditiveIntMod::operator+(const AdditiveIntMod& other) const {
    AdditiveIntMod result = *this;
    return result += other;
}

AdditiveIntMod AdditiveIntMod::operator-(const AdditiveIntMod& other) const {
    AdditiveIntMod result = *this;
    return result -= other;
}

AdditiveIntMod& AdditiveIntMod::operator+=(const int other) {
    return *this += AdditiveIntMod(other, _modulus);
}

AdditiveIntMod& AdditiveIntMod::operator-=(const int other) {
    return *this -= AdditiveIntMod(other, _modulus);
}

AdditiveIntMod AdditiveIntMod::operator+(const int other) const {
    AdditiveIntMod result = *this;
    return result += other;
}

AdditiveIntMod AdditiveIntMod::operator-(const int other) const {
    AdditiveIntMod result = *this;
    return result -= other;
}

std::strong_ordering AdditiveIntMod::operator<=>(const AdditiveIntMod& other) const {
    return _value <=> other._value;
}

std::strong_ordering AdditiveIntMod::operator<=>(const int other) const {
    return *this <=> AdditiveIntMod(other, _modulus);
}

bool AdditiveIntMod::operator==(const AdditiveIntMod& other) const {
    return _value == other._value;
}

bool AdditiveIntMod::operator==(const int other) const {
    return *this == AdditiveIntMod(other, _modulus);
}
