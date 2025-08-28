CXX = g++
CXXFLAGS = -std=c++17 -Wall $(shell pkg-config --cflags libmongocxx)
LDFLAGS = $(shell pkg-config --libs libmongocxx)

# Define os diretórios do projeto
SRC_ROOT_DIR = .
SRC_MODELS_DIR = projeto/models
SRC_VIEWS_DIR = projeto/views
BUILD_DIR = ./build
TARGET = sistema

# Define o nome do executável com a extensão .exe no Windows
ifeq ($(OS),Windows_NT)
  TARGET := $(TARGET).exe
endif

# Define o shell a ser usado e comandos específicos para o sistema operacional
ifeq ($(OS),Windows_NT)
  SHELL := cmd.exe
  CREATE_DIR_CMD = if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"
  CLEAN_CMD = rmdir /s /q "$(BUILD_DIR)" >nul 2>&1 & del "$(TARGET)" >nul 2>&1 & del usuarios_db.txt >nul 2>&1
  RUN_CMD = cmd /c ".\$(TARGET)"
else
  SHELL := /bin/sh
  CREATE_DIR_CMD = mkdir -p $(BUILD_DIR)
  CLEAN_CMD = rm -rf $(BUILD_DIR) $(TARGET) usuarios_db.txt
  RUN_CMD = ./$(TARGET)
endif

# Define todos os arquivos de código-fonte
# Esta abordagem usa a função 'wildcard' de forma abrangente para encontrar todos os arquivos .cpp
SOURCES = $(wildcard $(SRC_ROOT_DIR)/*.cpp) \
          $(wildcard $(SRC_MODELS_DIR)/*.cpp) \
          $(wildcard $(SRC_VIEWS_DIR)/*.cpp)

# A regra principal para construir o executável
# Compila e vincula todos os arquivos de uma vez.
$(TARGET): $(SOURCES)
	@echo "Compilando e vinculando todos os arquivos de uma vez..."
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS)

# Declara as targets 'all', 'run' e 'clean' como PHONY.
.PHONY: all run clean

all: run

# A target 'run' garante que o executável esteja construído e então o executa.
run: $(TARGET)
	@echo "Compilação concluída. Executando o programa..."
	$(RUN_CMD)

clean:
	@echo "Limpando os artefatos de build..."
	$(CLEAN_CMD)
