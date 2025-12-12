#include "solution.h"

#include <iostream>

std::istream& solution::operator>>(std::istream& is, solution::Rotation& rotation) {
    static const char LEFT_CHAR = 'L';
    static const char RIGHT_CHAR = 'R';

    char dirChar;
    unsigned value;

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

unsigned solution::Day1::solvePart1() const {
    std::unique_ptr<std::istream> input = _getInput();

    int value = _initValue;
    unsigned password = 0;
    Rotation rotation;
    while (*input >> rotation) {
        switch (rotation.direction) {
        case Direction::LEFT:
            value -= rotation.value;
            break;
        case Direction::RIGHT:
            value += rotation.value;
            break;
        }
        if (value % _modulus == 0) password++;
    }
    return password;
}

unsigned solution::Day1::solvePart2() const {
    std::unique_ptr<std::istream> input = _getInput();

    int value = _initValue;
    unsigned password = 0;
    Rotation rotation;
    while (*input >> rotation) {
        const int turns = rotation.value / _modulus;
        password += turns;
        rotation.value -= turns * _modulus;
        if (rotation.value == 0) continue;
        /* We are now away from 0 since  */

        const int before = value;
        bool passedOrEndedAtZero = false;
        switch (rotation.direction) {
        case Direction::LEFT:
            value -= rotation.value;
            passedOrEndedAtZero = (value - 1) % _modulus >= (before % _modulus);
            break;
        case Direction::RIGHT:
            value += rotation.value;
            passedOrEndedAtZero = (value + 1) % _modulus <= (before % _modulus);
            break;
        }
        /* Since we just rotated by less than a turn, the condition below can only be
         * verified if at or passed zero */
        const bool passedOrAtZero = (value - 1) % _modulus >= before;
        if (passedOrAtZero) password++;
    }
    return password;
}