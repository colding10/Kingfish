# ========== PROJECT PROPERTIES ==========
BIN_DIR = bin
INCLUDE_DIR = ./include
SRC_DIR := ./src
OBJ_DIR := obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

INCLUDE_DIRS = -I /usr/local/include 
LDFLAGS := -pthread -D_THREAD_SAFE -lSDL2 -lSDL2_image -lSDL2_ttf -L /usr/local/lib
CPPFLAGS := -g -I $(INCLUDE_DIR) $(INCLUDE_DIRS) -Wall -Wextra -std=c++11 -flto -O3 -march=native

CPP := g++-12


# ========== TARGETS ==========
$(BIN_DIR)/kingfish: $(OBJ_FILES)
	$(CPP) $(LDFLAGS) -o $@ ./$^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

clean: 
	rm $(OBJ_DIR)/*.o
	rm $(BIN_DIR)/kingfish