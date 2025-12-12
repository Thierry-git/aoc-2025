MAIN=main.cpp
LIB=libsolution.a

CC=g++
CFLAGS=-std=c++26 -Wall -Wextra -Werror -pedantic
AR=ar
ARFLAGS=rcs

main: lib
	$(CC) $(CFLAGS) -o $(MAIN:.cpp=) $(MAIN) -L. -lsolution
	
lib: solution.o
	$(AR) $(ARFLAGS) $(LIB) solution.o

solution.o: solution.cpp solution.h
	$(CC) $(CFLAGS) -c solution.cpp


clean:
	rm -f *.o *.a main

.PHONY: lib main clean