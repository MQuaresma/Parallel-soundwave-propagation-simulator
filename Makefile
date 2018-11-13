CC=gcc
CFLAGS=-O2
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin

SRC=$(wildcard $(SRC_DIR)/*.c)

all:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC)

clean:
	rm -rf $(BUILD_DIR)/ $(BIN_DIR)/
