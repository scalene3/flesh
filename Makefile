CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS =

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin


TARGET := $(BIN_DIR)/flesh

SRCS := $(shell find $(SRC_DIR) -name '*.c')

OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJS) $(LDFLAGS) -o $(TARGET)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(BUILD_DIR) $(BIN_DIR)
