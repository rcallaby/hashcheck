# Compiler and build flags
ifeq ($(OS),Windows_NT)
CXX ?= g++
else ifeq ($(shell uname -s),Darwin)
CXX ?= clang++
else
CXX ?= g++
endif
CPPFLAGS ?=
CXXFLAGS ?= -Wall -Wextra -O2 -std=c++17
LDFLAGS ?=
LDLIBS ?=

# Project structure
SRC := $(wildcard *.cpp)
OBJ := $(SRC:.cpp=.o)
DEP := $(OBJ:.o=.d)
ifeq ($(OS),Windows_NT)
BIN ?= hashcheck-rc.exe
RM := del /Q
RUN_PREFIX :=
TEST_EXIT := exit /b 2
else
BIN ?= hashcheck-rc
RM := rm -f
RUN_PREFIX := ./
TEST_EXIT := exit 2
endif

# Set TEST_CMD to the project's test runner, for example:
#   make test TEST_CMD="./tests/run.sh" (Linux/macOS)
#   make test TEST_CMD="tests\\run.bat" (Windows)
TEST_CMD ?=
ARGS ?=

.DEFAULT_GOAL := all
.PHONY: all build test check run clean rebuild help

# Keep built artifacts from being considered up to date if compilation fails
.DELETE_ON_ERROR:

# Default target
all: $(BIN)

# Explicit alias for the default build.
build: all

# Link object files into the final binary
$(BIN): $(OBJ)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compile each source file and generate dependency information
%.o: %.cpp
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@

# Include generated dependency files
-include $(DEP)

# Run the configured test command after building the program.
test: $(BIN)
ifeq ($(TEST_CMD),)
	@echo 'No test command configured; set TEST_CMD, e.g. make test TEST_CMD="./tests/run.sh"'
	@$(TEST_EXIT)
else
	$(TEST_CMD)
endif

check: test

# Run the program, optionally passing arguments with ARGS="...".
run: $(BIN)
	$(RUN_PREFIX)$(BIN) $(ARGS)

# Clean build artifacts
clean:
	$(RM) $(OBJ) $(DEP) $(BIN)

# Use recursive make so clean completes before building, even with -j.
rebuild:
	+$(MAKE) clean
	+$(MAKE) all

help:
	@echo 'Targets:'
	@echo '  all, build  Build $(BIN) (default)'
	@echo '  test       Run tests (configure with TEST_CMD=...)'
	@echo '  check      Alias for test'
	@echo '  run        Run $(BIN), optionally with ARGS="..."'
	@echo '  clean      Remove generated objects, dependencies, and binary'
	@echo '  rebuild    Clean and build again'
	@echo '  help       Show this help'
