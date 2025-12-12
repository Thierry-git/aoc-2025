#include "solution.h"

#include <iostream>

std::istream& solution::operator>>(std::istream& is, solution::Rotation& rotation) {
    static const char LEFT_CHAR = 'L';
    static const char RIGHT_CHAR = 'R';

    char dirChar;
    int value;

    if (is >> dirChar >> value) {
        Direction dir;
        switch (dirChar) {
        case LEFT_CHAR:
            dir = Direction::LEFT;
            break;
        case RIGHT_CHAR:
            dir = Direction::RIGHT;
            break;
        default:
            is.setstate(std::ios::failbit);
            return is;
        }
        rotation = { dir, value };
    }

    return is;
}

unsigned solution::Day1::solve() const {
    std::unique_ptr<std::istream> input = _getInput();

    AdditiveIntMod value = { _initValue, _modulus };
    unsigned password = 0;
    Rotation rotation;
    while (*input >> rotation) {
        password += rotate(value, rotation);
    }
    return password;
}

unsigned solution::Day1Part1::rotate(
    AdditiveIntMod& current, const Rotation& rotation) const {
    switch (rotation.direction) {
    case Direction::LEFT:
        current -= rotation.value;
        break;
    case Direction::RIGHT:
        current += rotation.value;
        break;
    }
    return current == 0 ? 1 : 0;
}

unsigned solution::Day1Part2::rotate(
    AdditiveIntMod& current, const Rotation& rotation) const {
    unsigned count = 0;

    const bool startedAtZero = (current == 0);

    const int rotRemainder = rotation.value % _modulus;
    const int turns = rotation.value / _modulus;

    count += turns;

    bool passedOrEndedAtZero;
    switch (rotation.direction) {
    case Direction::LEFT:
        passedOrEndedAtZero = rotRemainder >= current.distanceToZeroMovingLeft();
        current -= rotRemainder;
        break;
    case Direction::RIGHT:
        passedOrEndedAtZero = rotRemainder >= current.distanceToZeroMovingRight();
        current += rotRemainder;
        break;
    }
    if (passedOrEndedAtZero) count++;

    return count;
}
