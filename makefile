

BIN_DIR = bin
SRC_DIR = src

CAP1_DIR = $(SRC_DIR)/cap1
CAP3_DIR = $(SRC_DIR)/cap3

CC = gcc
CXX = g++

#make para todo
all: cap1 cap3


# Compilar cap1

cap1: $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp
	@mkdir -p $(BIN_DIR)/cap1
	$(CXX) -o $(BIN_DIR)/cap1/reciprocal $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp

# Compilar cap3 (varios listings)

CAP3_SRCS := $(wildcard $(CAP3_DIR)/listing3-*.c)
CAP3_BINS := $(patsubst $(CAP3_DIR)/%.c,$(BIN_DIR)/cap3/%,$(CAP3_SRCS))

cap3: $(CAP3_BINS)

# Regla genérica: de cada .c a su ejecutable en bin/cap3
# "|" indica order-only prerequisite: la carpeta se crea antes, pero no fuerza recompilación
$(BIN_DIR)/cap3/%: $(CAP3_DIR)/%.c | $(BIN_DIR)/cap3
	$(CC) -o $@ $<

# Target para crear la carpeta bin/cap3
$(BIN_DIR)/cap3:
	mkdir -p $(BIN_DIR)/cap3

# Limpieza de todo lo generado

clean: 
	rm -rf $(BIN_DIR)/*
