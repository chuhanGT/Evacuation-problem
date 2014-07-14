# This makefile builds a generic tester for all heaps


all: sim

CXX				   = g++

# add optimization, warning, debug, ... flags
CPPFLAGS        += -O3 -Wall -g -fno-inline -std=c++0x


# build your application
sim: TrafficSignal.o Trace.o Path.o RNG.o Pedestrian.o Cell.o CellularAutomata.o main.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $^ -o $@ $(LDLIBS)

main.o: main.cpp $(PA)/PolynomialArithmetic.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c main.cpp

TrafficSignal.o: TrafficSignal.h TrafficSignal.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c TrafficSignal.cpp

Trace.o: Trace.h Trace.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c Trace.cpp

RNG.o: RNG.h RNG.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c RNG.cpp

Path.o: Path.h Path.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c Path.cpp

Pedestrian.o: Pedestrian.cpp Pedestrian.h Direction.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c Pedestrian.cpp

CellularAutomata.o: CellularAutomata.cpp CellularAutomata.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c CellularAutomata.cpp

Cell.o: Cell.cpp Cell.h
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c Cell.cpp


clean:
	rm -f *.o sim