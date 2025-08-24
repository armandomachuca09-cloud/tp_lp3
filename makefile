

BIN_DIR = bin
SRC_DIR = src

CAP1_DIR = $(SRC_DIR)/cap1
CAP3_DIR = $(SRC_DIR)/cap3
CAP4_DIR = $(SRC_DIR)/cap4
CAP5_DIR = $(SRC_DIR)/cap5

CC = gcc
CXX = g++

#make para todo
all: cap1 cap3 cap4 cap5


# Compilar cap1------------------------------------------------------------------------

cap1: $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp
	@mkdir -p $(BIN_DIR)/cap1
	$(CXX) -o $(BIN_DIR)/cap1/reciprocal $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp

# compilar cap3-------------------------------------------------------------------------------------

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





#Compilar el cap4----------------------------------------------------------------------------------------

CAP4_SRCS_C := $(wildcard $(CAP4_DIR)/listing4-*.c)
CAP4_BINS_C := $(patsubst $(CAP4_DIR)/%.c,$(BIN_DIR)/cap4/%,$(CAP4_SRCS_C))

# Listado C++, para el caso 4-9.cpp 
CAP4_SRCS_CPP := $(CAP4_DIR)/listing4-9.cpp
CAP4_BINS_CPP := $(patsubst $(CAP4_DIR)/%.cpp,$(BIN_DIR)/cap4/%,$(CAP4_SRCS_CPP))

CAP4_BINS := $(CAP4_BINS_C) $(CAP4_BINS_CPP)

# Target principal
cap4: $(CAP4_BINS)

# Crear carpeta si no existe
$(BIN_DIR)/cap4:
	mkdir -p $(BIN_DIR)/cap4

# Reglas genéricas
$(BIN_DIR)/cap4/%: $(CAP4_DIR)/%.c | $(BIN_DIR)/cap4
	$(CC) -o $@ $<

$(BIN_DIR)/cap4/%: $(CAP4_DIR)/%.cpp | $(BIN_DIR)/cap4
	$(CXX) -o $@ $<
    
# Limpiar cap4
clean-cap4:
	rm -f $(BIN_DIR)/cap4/*
#Compilar cap 5---------------------------------------------------------------

# Limpieza de todo lo generado-------------------------
# Listados de C
CAP5_SRCS := $(wildcard $(CAP5_DIR)/listing5-*.c)
CAP5_BINS := $(patsubst $(CAP5_DIR)/%.c,$(BIN_DIR)/cap5/%,$(CAP5_SRCS))

# Target principal
cap5: $(CAP5_BINS)

# Crear carpeta bin/cap5 si no existe
$(BIN_DIR)/cap5:
	mkdir -p $(BIN_DIR)/cap5

# Regla genérica para compilar cada .c en su ejecutable
$(BIN_DIR)/cap5/%: $(CAP5_DIR)/%.c | $(BIN_DIR)/cap5
	$(CC) -o $@ $<

# Limpiar cap5
clean-cap5:
	rm -f $(BIN_DIR)/cap5/*




clean: 
	rm -rf $(BIN_DIR)/*
