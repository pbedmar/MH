SRC=src
OBJ=obj
BIN=bin
CXXFLAGS = -g -O3 -std=c++17

all: practica1

practica1: $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/functions.o
	g++ $(CXXFLAGS) $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/functions.o -o $(BIN)/practica1


$(OBJ)/practica1.o: src/practica1.cpp
	g++ $(CXXFLAGS) -c $(SRC)/practica1.cpp -o $(OBJ)/practica1.o


$(OBJ)/functions.o: src/functions.cpp
	g++ $(CXXFLAGS) -c $(SRC)/functions.cpp -o $(OBJ)/functions.o

$(OBJ)/DataLoader.o: $(SRC)/DataLoader.cpp $(SRC)/DataLoader.h
	g++ $(CXXFLAGS) -c $(SRC)/DataLoader.cpp -o $(OBJ)/DataLoader.o -I$(SRC)

$(OBJ)/GreedyAlgorithm.o: $(SRC)/GreedyAlgorithm.cpp $(SRC)/GreedyAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/GreedyAlgorithm.cpp -o $(OBJ)/GreedyAlgorithm.o -I$(SRC)


#$(OBJ)/random.o: $(SRC)/random.cpp $(SRC)/random.h
#	g++ $(FLAGS) -c $(SRC)/random.cpp -o $(OBJ)/random.o -I$(SRC)


clean:
	rm $(OBJ)/*.o $(BIN)/*