CXX = g++
CXXFLAGS = -Wall -O -std=c++11

all: rjson

rjson.o: src/rjson.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rjson: rjson.o src/main.cpp
	$(CXX) $(CXXFLAGS) -o rjson src/main.cpp rjson.o

clean:
	rm -f rjson *.o
