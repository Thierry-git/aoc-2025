#include "solution.h"

std::unique_ptr<std::istream> solution::Day2::getInputStream() {
    std::unique_ptr<std::ifstream> input = std::make_unique<std::ifstream>(_inputFile);
    if (!input || !*input) throw std::runtime_error("Input file not found");
    return input;
}