#include "solution.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <set>
#include <string>

#define CONFIG_FILE_PATH "config.txt"

#define INPUT_PATH_ARG "inputPath"
#define MODULUS_ARG "modulus"
#define INIT_VALUE_ARG "initValue"

std::string inputPath;
unsigned initValue;
unsigned modulus;

enum class Part {
    PART1,
    PART2,
};

std::optional<Part> partFromString(const std::string& str) {
    if (str == "part1") {
        return Part::PART1;
    } else if (str == "part2") {
        return Part::PART2;
    } else {
        return std::nullopt;
    }
}

/**
 * @returns true if all required args were set
 */
bool setConfig() {
    static const std::string CONFIG_LOG_PREFIX = "[CONFIG] ";

    std::set<std::string> args { INPUT_PATH_ARG, INIT_VALUE_ARG, MODULUS_ARG };

    std::ifstream configFile(CONFIG_FILE_PATH);
    if (!configFile) return false;

    std::string line;
    while (configFile >> line && args.size() > 0) {
        size_t delimPos = line.find('=');
        if (delimPos == std::string::npos) continue;
        std::string key = line.substr(0, delimPos);
        std::string value = line.substr(delimPos + 1);

        if (key == INPUT_PATH_ARG) {
            inputPath = value;
            args.erase(INPUT_PATH_ARG);
        } else if (key == INIT_VALUE_ARG) {
            initValue = std::stoi(value);
            args.erase(INIT_VALUE_ARG);
        } else if (key == MODULUS_ARG) {
            modulus = static_cast<unsigned>(std::stoul(value));
            args.erase(MODULUS_ARG);
        } else {
            continue;
        }
        std::cout << CONFIG_LOG_PREFIX << key << " = " << value << std::endl;
    }

    const bool success = args.size() == 0;

    std::cout << (success ? CONFIG_LOG_PREFIX + "All required config values set"
                          : CONFIG_LOG_PREFIX + "Missing required config values:");
    for (const auto& arg : args) {
        std::cout << " " << arg;
    }
    std::cout << std::endl;

    return success;
}

int main(int argc, char* argv[]) {
    if (!setConfig()) {
        std::cerr << "[ERROR] Failed to initialize from config file: aborting..."
                  << std::endl;
        return EXIT_FAILURE;
    }

    if (argc < 2) {
        std::cerr << "[ERROR] Missing part argument" << std::endl;
        std::cerr << "        Expected 'part1' or 'part2'" << std::endl;
        return EXIT_FAILURE;
    }

    const std::optional<Part> part = partFromString(argv[1]);
    if (!part) {
        std::cerr << "[ERROR] Invalid part argument: " << argv[1] << std::endl;
        std::cerr << "        Expected 'part1' or 'part2'" << std::endl;
        return EXIT_FAILURE;
    }

    std::unique_ptr<solution::Day1> solver;
    switch (*part) {
    case Part::PART1:
        solver = std::make_unique<solution::Day1Part1>(inputPath, initValue, modulus);
        break;
    case Part::PART2:
        solver = std::make_unique<solution::Day1Part2>(inputPath, initValue, modulus);
        break;
    }

    try {
        const unsigned password = solver->solve();
        std::cout << "[LOG] The password is: " << password << std::endl;
    } catch (const std::runtime_error& e) {
        std::cerr << "[ERROR] " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}