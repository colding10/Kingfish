# ========== PROJECT PROPERTIES ==========
BIN_DIR = bin
INCLUDE_DIR = ./include
SRC_DIR := ./src
OBJ_DIR := obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

INCLUDE_DIRS = -I /usr/local/include -I /usr/include
LDFLAGS := -pthread
CPPFLAGS := -g -pg -I $(INCLUDE_DIR) $(INCLUDE_DIRS) -Wall -Wextra -std=c++11 -flto -Ofast -march=native

CPP := g++-12


# ========== TARGETS ==========
$(BIN_DIR)/kingfish: $(OBJ_FILES)
	$(CPP) $(LDFLAGS) -o $@ ./$^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

format: $(SRC_DIR) $(INCLUDE_DIR)
	clang-format -i --style=file $(SRC_DIR)/*.cpp
	clang-format -i --style=file $(INCLUDE_DIR)/*.hpp

clean: 
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/kingfish