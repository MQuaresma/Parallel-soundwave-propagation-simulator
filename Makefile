CC=gcc
CFLAGS=-O2 -fopenmp
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin
SEQ=stencil_seq
OPT1=stencil_seq_opt

SRC=$(wildcard $(SRC_DIR)/*.c)

.DEFAULT_GOAL = all

$(BIN_DIR)/$(SEQ): $(SRC_DIR)/$(SEQ).c
	$(CC) $(CFLAGS) $(SRC_DIR)/$(SEQ).c -o $(SEQ)
	mv $(SEQ) $(BIN_DIR)

$(BIN_DIR)/$(OPT1): $(SRC_DIR)/$(OPT1).c
	$(CC) $(CFLAGS) $(SRC_DIR)/$(OPT1).c -o $(OPT1)
	mv $(OPT1) $(BIN_DIR)

checkdirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

all: checkdirs $(BIN_DIR)/$(SEQ) $(BIN_DIR)/$(OPT1)

clean:
	rm -rf $(BUILD_DIR)/ $(BIN_DIR)/
