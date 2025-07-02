CXX = g++
CXXFLAGS = -std=c++17 -Wall -Iinclude -pthread

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Recursively find all .cpp files in src and subfolders
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
# Replace src/xxx.cpp with obj/xxx.o for all sources
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

TARGET = $(BIN_DIR)/httpserver

all: dirs $(TARGET)

dirs:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Pattern rule to build .o files in obj/ mirroring src/ structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -f $(BIN_DIR)/httpserver

.PHONY: all clean dirs
