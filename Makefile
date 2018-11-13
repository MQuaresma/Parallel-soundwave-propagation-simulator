CC=gcc
CFLAGS=-O2
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin
SEQ=stencil_seq

SRC=$(wildcard $(SRC_DIR)/*.c)

all:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC)
	mv a.out $(BIN_DIR)

$(SEQ):$(SRC_DIR)/$(SEQ).c
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(SEQ).c -o $(SEQ)
	mv $(SEQ) $(BIN_DIR)

clean:
	rm -rf $(BUILD_DIR)/ $(BIN_DIR)/
