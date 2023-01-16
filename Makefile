# ========== PROJECT PROPERTIES ==========
BIN_DIR = bin
INCLUDE_DIR = ./include
SRC_DIR := ./src
OBJ_DIR := obj
LIB_DIR := ./libs

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -D_THREAD_SAFE -lSDL2 -lSDL2_image -lSDL2_ttf -L$(LIB_DIR)
CPPFLAGS := -I $(INCLUDE_DIR) -Wall -Wextra -std=c++11 

CPP := g++
# ========== PROJECT PROPERTIES ==========


$(BIN_DIR)/main: $(OBJ_FILES)
	$(CPP) $(LDFLAGS) -o $@ ./$^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/main