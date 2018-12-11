CC=mpicc
CFLAGS=-O3 -std=c99
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin
MPI=stencil_mpi
DEPS=matrix_utils

SRC=$(wildcard $(SRC_DIR)/*.c)

.DEFAULT_GOAL = all

$(DEPS): $(wildcard $(SRC_DIR)/$(DEPS).[hc])

$(BIN_DIR)/$(MPI): $(SRC_DIR)/$(MPI).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(MPI).c $(SRC_DIR)/$(DEPS).c
	mv a.out $(BIN_DIR)

checkdirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

all: checkdirs $(BIN_DIR)/$(MPI)

clean:
	rm -rf $(BUILD_DIR)/ $(BIN_DIR)/
