SRC=src
OBJ=obj
BIN=bin
FLAGS = -O3 -std=c++14

all: practica1

practica1: $(OBJ)/practica1.o $(OBJ)/DataLoader.o
	g++ $(FLAGS) $(OBJ)/practica1.o $(OBJ)/DataLoader.o -o $(BIN)/practica1

$(OBJ)/practica1.o: src/practica1.cpp
	g++ $(FLAGS) -c $(SRC)/practica1.cpp -o $(OBJ)/practica1.o

$(OBJ)/DataLoader.o: $(SRC)/DataLoader.cpp $(SRC)/DataLoader.h
	g++ $(FLAGS) -c $(SRC)/DataLoader.cpp -o $(OBJ)/DataLoader.o -I$(SRC)