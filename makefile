# Compiladores y opciones
CC = gcc
CXX = g++
CFLAGS = -Wall -g
CXXFLAGS = -Wall -g

# Carpetas
SRC_DIR = src/cap1
BIN_DIR = bin/cap1

# Archivos fuente C y C++
CFILES := $(wildcard $(SRC_DIR)/*.c)
CPPFILES := $(wildcard $(SRC_DIR)/*.cpp)

# Archivos objeto
C_OBJS := $(CFILES:$(SRC_DIR)/%.c=$(BIN_DIR)/%.o)
CPP_OBJS := $(CPPFILES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%.o)

# Ejecutables finales (un por cada listing que tenga main)
# Para este ejemplo, asumimos que main.c es el principal
BIN = $(BIN_DIR)/reciprocal

# ===== Reglas =====

# Compilar todo
all: $(BIN)

# Enlazar ejecutables
$(BIN): $(C_OBJS) $(CPP_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(C_OBJS) $(CPP_OBJS) -o $(BIN)
	@echo "Compilado exitoso: $(BIN)"

# Compilar C
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar C++
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpiar ejecutables y objetos
clean:
	rm -rf $(BIN_DIR)/*
	@echo "Limpieza completada"

# Decirle a Make que estas reglas no son archivos reales
.PHONY: all clean

