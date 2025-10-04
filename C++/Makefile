CXX = g++
CXXFLAGS = -std=c++17 -Wall $(shell pkg-config --cflags libmongocxx)
LDFLAGS = $(shell pkg-config --libs libmongocxx)

TARGET = sistema

ifeq ($(OS),Windows_NT)
  TARGET := $(TARGET).exe
endif

# Encontra todos os arquivos .cpp na pasta atual e em subpastas
SOURCES = $(shell find . -name "*.cpp")

# Regra principal para construir o executável
all: $(TARGET)

$(TARGET): $(SOURCES)
	@echo "Compilando e vinculando fontes..."
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@ $(LDFLAGS)

run: all
	@echo "Executando o programa..."
	./$(TARGET)

clean:
	@echo "Limpando artefatos de build..."
	rm -f $(TARGET)
	rm -f *.o

.PHONY: all run clean