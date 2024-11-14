# directories
SRC_DIR  = src
TEST_DIR = test
BIN_DIR  = bin
INC_DIR  = include

# compiler & flags
SHELL       = /bin/sh
CC          = gcc
CFLAGS      = -g -Wall -I$(INC_DIR)
TEST_CFLAGS = $(CFLAGS) -Wno-unused-function
RM          = rm -f

# ensure ./bin exists
$(shell mkdir -p $(BIN_DIR))

# src files
SRC      = $(wildcard $(SRC_DIR)/*.c)
TEST_SRC = $(wildcard $(TEST_DIR)/*.c)

# binary names
PROG_NAME = pangolin
TEST_NAME = test

.PHONY: all
all: $(BIN_DIR)/$(PROG_NAME)

$(BIN_DIR)/$(PROG_NAME): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^

.PHONY: test
test: $(BIN_DIR)/$(TEST_NAME)
	@./$(BIN_DIR)/$(TEST_NAME)

$(BIN_DIR)/$(TEST_NAME): $(TEST_SRC) $(SRC)
	$(CC) $(TEST_CFLAGS) -o $@ $^

.PHONY: clean
clean:
	$(RM) -r $(BIN_DIR)