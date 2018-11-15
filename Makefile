CC=gcc
CFLAGS=-O2
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin
SEQ=stencil_seq
OPT1=stencil_seq_opt
DEPS=matrix_utils
OS:= $(shell uname -s)


ifeq ($(OS), Darwin)
	CFLAGS += -Xpreprocessor -fopenmp -lomp
else
	CFLAGS += -fopenmp
endif

SRC=$(wildcard $(SRC_DIR)/*.c)

.DEFAULT_GOAL = all

$(DEPS): $(wildcard $(SRC_DIR)/$(DEPS).[hc])

$(BIN_DIR)/$(SEQ): $(SRC_DIR)/$(SEQ).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(SEQ).c $(SRC_DIR)/$(DEPS).c -o $(SEQ)
	mv $(SEQ) $(BIN_DIR)

$(BIN_DIR)/$(OPT1): $(SRC_DIR)/$(OPT1).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(OPT1).c $(SRC_DIR)/$(DEPS).c -o $(OPT1)
	mv $(OPT1) $(BIN_DIR)

checkdirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

all: checkdirs $(BIN_DIR)/$(SEQ) $(BIN_DIR)/$(OPT1)

clean:
	rm -rf $(BUILD_DIR)/ $(BIN_DIR)/
