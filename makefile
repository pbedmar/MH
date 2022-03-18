SRC=src
OBJ=obj
BIN=bin
FLAGS = -g -O3 -std=c++14

all: practica1

practica1: $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/random.o
	g++ $(FLAGS) $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o -o $(BIN)/practica1

$(OBJ)/practica1.o: src/practica1.cpp
	g++ $(FLAGS) -c $(SRC)/practica1.cpp -o $(OBJ)/practica1.o


$(OBJ)/DataLoader.o: $(SRC)/DataLoader.cpp $(SRC)/DataLoader.h
	g++ $(FLAGS) -c $(SRC)/DataLoader.cpp -o $(OBJ)/DataLoader.o -I$(SRC)

$(OBJ)/GreedyAlgorithm.o: $(SRC)/GreedyAlgorithm.cpp $(SRC)/GreedyAlgorithm.h
	g++ $(FLAGS) -c $(SRC)/GreedyAlgorithm.cpp -o $(OBJ)/GreedyAlgorithm.o -I$(SRC)


#$(OBJ)/random.o: $(SRC)/random.cpp $(SRC)/random.h
#	g++ $(FLAGS) -c $(SRC)/random.cpp -o $(OBJ)/random.o -I$(SRC)


clean:
	rm $(OBJ)/*.o $(BIN)/*