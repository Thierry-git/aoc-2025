# Advent of Code - Day Makefile
# Usage:
#   make        - Build main executable
#   make test   - Build and run tests
#   make run1   - Build and run Part 1
#   make run2   - Build and run Part 2
#   make clean  - Remove build artifacts

CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Werror -pedantic -O2 -I..

# Executables
MAIN = main
TEST = test

# Source files
MAIN_SRC = main.cpp solution.cpp
TEST_SRC = test.cpp solution.cpp

# Object files
MAIN_OBJS = $(MAIN_SRC:.cpp=.o)
TEST_OBJS = $(TEST_SRC:.cpp=.o)

# Default target
all: $(MAIN)

# Main executable
$(MAIN): $(MAIN_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Test executable
$(TEST): $(TEST_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Generic compilation rule
%.o: %.cpp solution.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run targets
run1: $(MAIN)
	./$(MAIN) part1

run2: $(MAIN)
	./$(MAIN) part2

runtest: $(TEST)
	./$(TEST)

# Clean build artifacts
clean:
	rm -f $(MAIN) $(TEST) *.o

.PHONY: all clean run1 run2 runtest
