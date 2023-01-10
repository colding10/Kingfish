# ========== PROJECT PROPERTIES ==========
BIN = bin
INCLUDE = include
SOURCE = src
LIBS = libs


CPP = g++
CPPFLAGS = -Wall -Wextra -std=c++11
# ========== PROJECT PROPERTIES ==========


all: $(BIN)/prog

$(BIN)/prog: $(SOURCE)/main.cpp $(SOURCE)/piece.cpp
	$(CPP) $(CPPFLAGS) -L $(LIBS) -I $(INCLUDE) -lSDL2 -o $(@) $(SOURCE)/*

$(BIN)/main: $(SOURCE)/main.cpp
	$(CPP) $(CPPFLAGS) -L $(LIBS) -I $(INCLUDE) -lSDL2 -o $(@) $(<)


clean: 
	rm -R $(BIN)
	mkdir $(BIN)