# Compiler and flags
CXX         = g++
CXXFLAGS    = -std=c++23 -Wall -Wextra -g -Isrc

# Directories
SRC_DIR     = src
TEST_DIR    = test
BUILD_DIR   = build
INC_DIR     = $(TEST_DIR)

# Main target
TARGET      = pangolin

# Source and object files
SRCS        = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS        = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(subst $(SRC_DIR)/, , $(SRCS)))

# Exclude main.cpp from test target
MAIN_SRC        = $(SRC_DIR)/main.cpp
MAIN_OBJ        = $(BUILD_DIR)/main.o
OBJS_NO_MAIN    = $(filter-out $(MAIN_OBJ), $(OBJS))

# Test sources
TEST_MAIN   = $(TEST_DIR)/test_main.cpp
TEST_SRCS   = $(filter-out $(TEST_MAIN), $(wildcard $(TEST_DIR)/test_*.cpp))
TEST_TARGET = $(BUILD_DIR)/run_tests

# Default target
all: $(TARGET)

# Build main binary
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Compile object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build test binary (exclude main.o)
$(TEST_TARGET): $(TEST_MAIN) $(TEST_SRCS) $(OBJS_NO_MAIN)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o $@ $^

# Run tests
test: $(TEST_TARGET)
	@echo "Running tests..."
	@./$(TEST_TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(TARGET)

.PHONY: all test clean
