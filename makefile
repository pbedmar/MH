SRC=FUENTES
OBJ=obj
BIN=BIN
CXXFLAGS = -O3 -std=c++17

all: practica1

practica1: $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/LocalSearchAlgorithm.o $(OBJ)/functions.o
	g++ $(CXXFLAGS) $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/LocalSearchAlgorithm.o $(OBJ)/functions.o -o $(BIN)/practica1


$(OBJ)/practica1.o: $(SRC)/practica1.cpp
	g++ $(CXXFLAGS) -c $(SRC)/practica1.cpp -o $(OBJ)/practica1.o


$(OBJ)/functions.o: $(SRC)/functions.cpp
	g++ $(CXXFLAGS) -c $(SRC)/functions.cpp -o $(OBJ)/functions.o

$(OBJ)/DataLoader.o: $(SRC)/DataLoader.cpp $(SRC)/DataLoader.h
	g++ $(CXXFLAGS) -c $(SRC)/DataLoader.cpp -o $(OBJ)/DataLoader.o -I$(SRC)

$(OBJ)/GreedyAlgorithm.o: $(SRC)/GreedyAlgorithm.cpp $(SRC)/GreedyAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/GreedyAlgorithm.cpp -o $(OBJ)/GreedyAlgorithm.o -I$(SRC)

$(OBJ)/LocalSearchAlgorithm.o: $(SRC)/LocalSearchAlgorithm.cpp $(SRC)/LocalSearchAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/LocalSearchAlgorithm.cpp -o $(OBJ)/LocalSearchAlgorithm.o -I$(SRC)

clean:
	rm $(OBJ)/*.o $(BIN)/*