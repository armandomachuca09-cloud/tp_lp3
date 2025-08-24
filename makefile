BIN_DIR = bin
SRC_DIR = src

CAP1_DIR = $(SRC_DIR)/cap1
CAP2_DIR = $(SRC_DIR)/cap2
CAP3_DIR = $(SRC_DIR)/cap3
CAP4_DIR = $(SRC_DIR)/cap4
CAP5_DIR = $(SRC_DIR)/cap5

CC = gcc
CXX = g++

#make para todo
all: cap1 cap2  cap3 cap4 cap5


# Compilar cap1------------------------------------------------------------------------

cap1: $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp
	@mkdir -p $(BIN_DIR)/cap1
	$(CXX) -o $(BIN_DIR)/cap1/reciprocal $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp

#compilar cap2---------------------------------------------------------------

# Listados normales (hay kilombo con los listings 2-7, 2-8, 2-9)
CAP2_SRCS_NORMAL := $(filter-out $(CAP2_DIR)/listing2-7.c $(CAP2_DIR)/listing2-8.c $(CAP2_DIR)/listing2-9.c,$(wildcard $(CAP2_DIR)/listing2-*.c))
CAP2_BINS_NORMAL := $(patsubst $(CAP2_DIR)/%.c,$(BIN_DIR)/cap2/%,$(CAP2_SRCS_NORMAL))

# Target principal 
cap2: $(BIN_DIR)/cap2/listing2-8 $(BIN_DIR)/cap2/listing2-9 $(CAP2_BINS_NORMAL)

# Crear carpeta bin/cap2 si no existe
$(BIN_DIR)/cap2:
	mkdir -p $(BIN_DIR)/cap2

# Reglas
# 2-7 + 2-8 compilados juntos
$(BIN_DIR)/cap2/listing2-8: $(CAP2_DIR)/listing2-7.c $(CAP2_DIR)/listing2-8.c | $(BIN_DIR)/cap2
	$(CC) -o $@ $(CAP2_DIR)/listing2-7.c $(CAP2_DIR)/listing2-8.c

# 2-9 con libtiff
$(BIN_DIR)/cap2/listing2-9: $(CAP2_DIR)/listing2-9.c | $(BIN_DIR)/cap2
	$(CC) -o $@ $< -ltiff

# Resto de listings normales
$(BIN_DIR)/cap2/%: $(CAP2_DIR)/%.c | $(BIN_DIR)/cap2
	$(CC) -o $@ $<

# Limpiar cap2
clean-cap2:
	rm -f $(BIN_DIR)/cap2/*


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
CAP4_SRCS_CPP := $(CAP4_DIR)/listing4-9.cpp

CAP4_BINS_C := $(patsubst $(CAP4_DIR)/%.c,$(BIN_DIR)/cap4/%,$(CAP4_SRCS_C))
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


#limpiar todo----------------------------------------------------------------

clean: 
	rm -rf $(BIN_DIR)/*

#compilar cada listing uno por uno----------------------------------------
#cap1--------------------------------------------------------
# Crear carpeta bin/cap1 si no existe
$(BIN_DIR)/cap1:
	mkdir -p $(BIN_DIR)/cap1

# Compilar el ejecutable cap1/reciprocal
$(BIN_DIR)/cap1/reciprocal: $(CAP1_DIR)/listing1-1.c $(CAP1_DIR)/listing1-2.cpp | $(BIN_DIR)/cap1
	$(CXX) -o $@ $^
#target principal
listing1-1: $(BIN_DIR)/cap1/reciprocal
#cap2---------------------------------------------------------------
listing2-8: $(BIN_DIR)/cap2/listing2-8
listing2-9: $(BIN_DIR)/cap2/listing2-9

#los que no dan kilombo en el cap2
$(foreach f,$(CAP2_SRCS_NORMAL),$(eval $(notdir $(basename $(f))): $(BIN_DIR)/cap2/$(notdir $(basename $(f)))) )
#cap3-------------------------------------------------------------
$(foreach f,$(CAP3_SRCS),$(eval $(notdir $(basename $(f))): $(BIN_DIR)/cap3/$(notdir $(basename $(f)))))
#cap4-------------------------------------------------------------
#cap5-------------------------------------------------------------
$(foreach f,$(CAP5_SRCS),$(eval $(notdir $(basename $(f))): $(BIN_DIR)/cap5/$(notdir $(basename $(f)))))
