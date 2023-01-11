# ========== PROJECT PROPERTIES ==========
BIN_DIR = ./bin
INCLUDE_DIR = ./include
SRC_DIR := ./src
OBJ_DIR := ./obj
LIB_DIR := ./libs

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -lSDL2 -lSDL2_image -L$(LIB_DIR)
CPPFLAGS := -I $(INCLUDE_DIR) -Wall -Wextra -std=c++11

CPP := g++
# ========== PROJECT PROPERTIES ==========


$(BIN_DIR)/main: $(OBJ_FILES)
	$(CPP) $(LDFLAGS) -o $@ ./$^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean: 
	find $(BIN_DIR) -name '*' -delete
	find  $(OBJ_DIR) -name '*' -delete