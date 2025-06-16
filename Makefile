CXX         = g++
CXXFLAGS    = -std=c++23 -Wall -Wextra -g

SRC_DIR     = src
TEST_DIR    = test
BUILD_DIR   = build
INC_DIR     = $(TEST_DIR)

SRCS        = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS        = $(patsubst %.cpp, $(BUILD_DIR)/%.o, $(subst $(SRC_DIR)/, , $(SRCS)))
TARGET      = pangolin

TEST_MAIN   = $(TEST_DIR)/test_main.cpp
TEST_SRCS   = $(filter-out $(TEST_MAIN), $(wildcard $(TEST_DIR)/test_*.cpp))
TEST_TARGET = run_tests

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_TARGET): $(TEST_MAIN) $(TEST_SRCS)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -o $@ $^

clean:
	rm -rf $(BUILD_DIR) $(TARGET) $(TEST_TARGET)

.PHONY: all test clean
