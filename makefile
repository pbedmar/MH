SRC=FUENTES
OBJ=obj
BIN=BIN
CXXFLAGS = -g -O3 -std=c++17

all: practica1 practica2 practica3

practica1: $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/LocalSearchAlgorithm.o $(OBJ)/functions.o
	g++ $(CXXFLAGS) $(OBJ)/practica1.o $(OBJ)/DataLoader.o $(OBJ)/GreedyAlgorithm.o $(OBJ)/LocalSearchAlgorithm.o $(OBJ)/functions.o -o $(BIN)/practica1

practica2: $(OBJ)/practica2.o $(OBJ)/DataLoader.o $(OBJ)/GeneticAlgorithm.o $(OBJ)/MemeticAlgorithm.o $(OBJ)/functions.o
	g++ $(CXXFLAGS) $(OBJ)/practica2.o $(OBJ)/DataLoader.o $(OBJ)/GeneticAlgorithm.o $(OBJ)/MemeticAlgorithm.o $(OBJ)/functions.o -o $(BIN)/practica2

practica3: $(OBJ)/practica3.o $(OBJ)/DataLoader.o $(OBJ)/TrajectoryAlgorithms.o $(OBJ)/functions.o
	g++ $(CXXFLAGS) $(OBJ)/practica3.o $(OBJ)/DataLoader.o $(OBJ)/TrajectoryAlgorithms.o $(OBJ)/functions.o -o $(BIN)/practica3

$(OBJ)/practica1.o: $(SRC)/practica1.cpp
	g++ $(CXXFLAGS) -c $(SRC)/practica1.cpp -o $(OBJ)/practica1.o

$(OBJ)/practica2.o: $(SRC)/practica2.cpp
	g++ $(CXXFLAGS) -c $(SRC)/practica2.cpp -o $(OBJ)/practica2.o

$(OBJ)/practica3.o: $(SRC)/practica3.cpp
	g++ $(CXXFLAGS) -c $(SRC)/practica3.cpp -o $(OBJ)/practica3.o


$(OBJ)/functions.o: $(SRC)/functions.cpp
	g++ $(CXXFLAGS) -c $(SRC)/functions.cpp -o $(OBJ)/functions.o

$(OBJ)/DataLoader.o: $(SRC)/DataLoader.cpp $(SRC)/DataLoader.h
	g++ $(CXXFLAGS) -c $(SRC)/DataLoader.cpp -o $(OBJ)/DataLoader.o -I$(SRC)

$(OBJ)/GreedyAlgorithm.o: $(SRC)/GreedyAlgorithm.cpp $(SRC)/GreedyAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/GreedyAlgorithm.cpp -o $(OBJ)/GreedyAlgorithm.o -I$(SRC)

$(OBJ)/LocalSearchAlgorithm.o: $(SRC)/LocalSearchAlgorithm.cpp $(SRC)/LocalSearchAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/LocalSearchAlgorithm.cpp -o $(OBJ)/LocalSearchAlgorithm.o -I$(SRC)

$(OBJ)/GeneticAlgorithm.o: $(SRC)/GeneticAlgorithm.cpp $(SRC)/GeneticAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/GeneticAlgorithm.cpp -o $(OBJ)/GeneticAlgorithm.o -I$(SRC)

$(OBJ)/MemeticAlgorithm.o: $(SRC)/MemeticAlgorithm.cpp $(SRC)/MemeticAlgorithm.h
	g++ $(CXXFLAGS) -c $(SRC)/MemeticAlgorithm.cpp -o $(OBJ)/MemeticAlgorithm.o -I$(SRC)

$(OBJ)/TrajectoryAlgorithms.o: $(SRC)/TrajectoryAlgorithms.cpp $(SRC)/TrajectoryAlgorithms.h
	g++ $(CXXFLAGS) -c $(SRC)/TrajectoryAlgorithms.cpp -o $(OBJ)/TrajectoryAlgorithms.o -I$(SRC)


clean:
	rm $(OBJ)/*.o $(BIN)/*