CXX ?= g++
CXXFLAGS ?= -std=c++17 -O2 -Wall -Wextra -Wpedantic
CPPFLAGS ?= -Isrc
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst src/%.cpp,obj/%.o,$(SOURCES))
TARGET := bin/hashcheck-rc
.PHONY: all clean test check integration-test
all: $(TARGET)
$(TARGET): $(OBJECTS)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $^ -o $@
obj/%.o: src/%.cpp
	@mkdir -p obj
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -MP -c $< -o $@
-include $(OBJECTS:.o=.d)
test: bin/detector_test $(TARGET)
	./bin/detector_test
	$(MAKE) integration-test
integration-test: $(TARGET) bin/integration_test bin/mock_hashcat
	./bin/integration_test $(TARGET) ./bin/mock_hashcat
check: test
bin/detector_test: tests/detector_test.cpp src/hashdetector.cpp src/HashDetector.h
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) tests/detector_test.cpp src/hashdetector.cpp -o $@
clean:
	rm -rf obj bin

bin/integration_test: tests/integration_test.cpp
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
bin/mock_hashcat: tests/mock_hashcat.cpp
	@mkdir -p bin
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
