CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

SRC_DIR = src
BUILD_DIR = build

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)
TARGET = tiger

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

clean:
	rm -rf $(BUILD_DIR) $(TARGET)

# Dependencies (simplified - for proper deps use -MMD -MP)
$(BUILD_DIR)/token.o: $(SRC_DIR)/token.hpp
$(BUILD_DIR)/lexer.o: $(SRC_DIR)/lexer.hpp $(SRC_DIR)/token.hpp
$(BUILD_DIR)/parser.o: $(SRC_DIR)/parser.hpp $(SRC_DIR)/ast.hpp $(SRC_DIR)/lexer.hpp $(SRC_DIR)/token.hpp
$(BUILD_DIR)/main.o: $(SRC_DIR)/lexer.hpp $(SRC_DIR)/parser.hpp
