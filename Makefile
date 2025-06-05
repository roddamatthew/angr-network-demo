# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files
SRC_FILES = $(SRC_DIR)/networking.c $(SRC_DIR)/reply-trigger.c

# Output binary
TARGET = $(BUILD_DIR)/main

# Default target
all: $(TARGET)

# Link and compile
$(TARGET): $(SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(TARGET)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean
