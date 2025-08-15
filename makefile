# Compilador y opciones
CC = gcc
CFLAGS = -Wall -g

# Carpetas
SRC_DIR = src/cap1
BIN_DIR = bin/cap1

# Archivo fuente y ejecutable
SRC = $(SRC_DIR)/listing1.1.c
BIN = $(BIN_DIR)/listing1.1

# ===== Reglas =====

# Compilar el listing 1.1
all: $(BIN)

$(BIN):
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $(SRC) -o $(BIN)
	@echo "Compilado: $(SRC) -> $(BIN)"

# Limpiar
clean:
	rm -rf $(BIN)
	@echo "Ejecutable borrado: $(BIN)"

# Decirle a Make que estas reglas no son archivos reales
.PHONY: all clean

