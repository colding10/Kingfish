# ========== PROJECT PROPERTIES ==========
BIN_DIR = bin
SRC_DIR := src
OBJ_DIR := obj

SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

LDFLAGS := -pthread
CPPFLAGS := -g -pg -target arm64-apple-darwin-gnu -std=c++2a -Wall -Wextra -flto -O3 -march=native

CPP := clang++


# ========== TARGETS ==========
$(BIN_DIR)/kingfish: $(OBJ_FILES)
	$(CPP) $(LDFLAGS) -o $@ ./$^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CPP) $(CPPFLAGS) -c -o $@ $<

format: $(SRC_DIR)
	clang-format -i --style=file $(SRC_DIR)/*.cpp
	clang-format -i --style=file $(SRC_DIR)/*.hpp

clean: 
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/kingfish