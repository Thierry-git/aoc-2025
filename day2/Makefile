CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2

TARGET = main
TEST = test
OBJS = main.o solution.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

main.o: main.cpp solution.h
	$(CXX) $(CXXFLAGS) -c main.cpp

solution.o: solution.cpp solution.h
	$(CXX) $(CXXFLAGS) -c solution.cpp

test: solution.o test.cpp
	$(CXX) $(CXXFLAGS) -o $(TEST) solution.o test.cpp

clean:
	rm -f $(TARGET) $(TEST) $(OBJS)

.PHONY: all clean
