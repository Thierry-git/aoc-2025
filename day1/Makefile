MAIN=main.cpp
TEST=test.cpp
LIB_DIR=lib
LIB=$(LIB_DIR)/libsolution.a

CC=g++
CFLAGS=-std=c++26 -Wall -Wextra -Werror -pedantic -I$(LIB_DIR)
AR=ar
ARFLAGS=rcs

main: lib
	$(CC) $(CFLAGS) -o $(MAIN:.cpp=) $(MAIN) -L$(LIB_DIR) -lsolution

test: lib
	$(CC) $(CFLAGS) -o $(TEST:.cpp=) $(TEST) -L$(LIB_DIR) -lsolution

lib: $(LIB)

$(LIB): $(LIB_DIR)/solution.o $(LIB_DIR)/additive_int_mod.o
	$(AR) $(ARFLAGS) $(LIB) $(LIB_DIR)/solution.o $(LIB_DIR)/additive_int_mod.o

$(LIB_DIR)/solution.o: $(LIB_DIR)/solution.cpp $(LIB_DIR)/solution.h $(LIB_DIR)/additive_int_mod.h
	$(CC) $(CFLAGS) -c $(LIB_DIR)/solution.cpp -o $(LIB_DIR)/solution.o

$(LIB_DIR)/additive_int_mod.o: $(LIB_DIR)/additive_int_mod.cpp $(LIB_DIR)/additive_int_mod.h
	$(CC) $(CFLAGS) -c $(LIB_DIR)/additive_int_mod.cpp -o $(LIB_DIR)/additive_int_mod.o

clean:
	rm -f $(LIB_DIR)/*.o $(LIB_DIR)/*.a main test

.PHONY: lib main test clean