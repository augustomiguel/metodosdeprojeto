CXX = g++
CXXFLAGS = -std=c++17 -Wall `pkg-config --cflags libmongocxx`
LDFLAGS = `pkg-config --libs libmongocxx`
SRC_DIR = ./projeto
BUILD_DIR = ./build
TARGET = sistema

SOURCES = $(wildcard $(SRC_DIR)/*.cpp) \
          $(wildcard $(SRC_DIR)/models/*.cpp) \
          $(wildcard $(SRC_DIR)/views/*.cpp)

OBJECTS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

all: $(TARGET)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)