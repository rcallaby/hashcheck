# Compiler and flags
CXX := g++
CXXFLAGS := -Wall -O2 -std=c++17

# Project structure
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
BIN := hashcheck-rc

# Default target
all: $(BIN)

# Link object files into final binary
$(BIN): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile each .cpp into .o
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJ) $(BIN)
