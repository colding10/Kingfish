# ========== PROJECT PROPERTIES ==========
BIN_DIR = bin
INCLUDE_DIR = ./include
SRC_DIR := ./src
OBJ_DIR := obj
LIB_DIR := ./libs

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -pthread -D_THREAD_SAFE -lSDL2 -lSDL2_image -lSDL2_ttf -L$(LIB_DIR) -L /usr/local/lib
CPPFLAGS := -g -I $(INCLUDE_DIR) -I /usr/local/include -Wall -Wextra -std=c++11 -flto -O3 -march=native

CPP := g++-12


# ========== TARGETS ==========
$(BIN_DIR)/main: $(OBJ_FILES)
	$(CPP) $(LDFLAGS) -o $@ ./$^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/main