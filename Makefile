# Makefile

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -O2

# Directories
SRC_DIR = src
BUILD_DIR = build

# Source files for main binary
MAIN_SRC_FILES = $(SRC_DIR)/networking.c $(SRC_DIR)/reply-trigger.c

# Source files for main-ip binary
MAIN_IP_SRC_FILES = $(SRC_DIR)/networking.c $(SRC_DIR)/reply-trigger-ip.c

# Output binaries
TARGET_MAIN = $(BUILD_DIR)/main
TARGET_MAIN_IP = $(BUILD_DIR)/main-ip

# Default target - build both binaries
all: $(TARGET_MAIN) $(TARGET_MAIN_IP)

# Link and compile main binary
$(TARGET_MAIN): $(MAIN_SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(MAIN_SRC_FILES) -o $(TARGET_MAIN)

# Link and compile main-ip binary
$(TARGET_MAIN_IP): $(MAIN_IP_SRC_FILES)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(MAIN_IP_SRC_FILES) -o $(TARGET_MAIN_IP)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean