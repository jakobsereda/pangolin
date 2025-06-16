CXX         = g++
CXXFLAGS    = -std=c++23 -Wall -Wextra -g

SRC_DIR     = src
TEST_DIR    = test
BUILD_DIR   = build
INC_DIR     = $(TEST_DIR)

SRCS        = $(wildcard $(SRC_DIR)/*.cpp)
OBJS        = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))
TARGET      = pangolin

TEST_MAIN   = $(TEST_DIR)/test_main.cpp
TEST_SRCS   = $(filter-out $(TEST_MAIN), $(wildcard $(TEST_DIR)/test_*.cpp))
TEST_TARGET = run_tests

all: $(TARGET)

$(TARGET): $(OBJS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_MAIN) $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o $@ $^

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

test: $(TEST_TARGET)
	@echo "Running tests..."
	@./$(TEST_TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all test clean

