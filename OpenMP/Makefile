CC=gcc
CFLAGS=-O3 -std=c99 -fopt-info-vec-optimized
SRC_DIR=src
BUILD_DIR=build
BIN_DIR=bin
SEQ=stencil_seq
OPT=stencil_seq_opt
PAR=stencil_parallel
SEQVECT=stencil_seq_vect
PARVECT=stencil_parallel_vect
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

$(BIN_DIR)/$(OPT): $(SRC_DIR)/$(OPT).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(OPT).c $(SRC_DIR)/$(DEPS).c -o $(OPT)
	mv $(OPT) $(BIN_DIR)

$(BIN_DIR)/$(PAR): $(SRC_DIR)/$(PAR).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(PAR).c $(SRC_DIR)/$(DEPS).c -o $(PAR)
	mv $(PAR) $(BIN_DIR)

$(BIN_DIR)/$(SEQVECT): $(SRC_DIR)/$(SEQVECT).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(SEQVECT).c $(SRC_DIR)/$(DEPS).c -o $(SEQVECT)
	mv $(SEQVECT) $(BIN_DIR)

$(BIN_DIR)/$(PARVECT): $(SRC_DIR)/$(PARVECT).c $(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(PARVECT).c $(SRC_DIR)/$(DEPS).c -o $(PARVECT)
	mv $(PARVECT) $(BIN_DIR)

checkdirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(BIN_DIR)

all: checkdirs $(BIN_DIR)/$(SEQ) $(BIN_DIR)/$(OPT) $(BIN_DIR)/$(PAR) $(BIN_DIR)/$(SEQVECT) $(BIN_DIR)/$(PARVECT)  

clean:
	rm -rf $(BUILD_DIR)/ $(BIN_DIR)/
