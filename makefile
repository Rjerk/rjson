CXX = g++
CXXFLAGS = -Wall -O -std=c++11

all: rjson

rjson.o: src/rjson.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

rjson: rjson.o main.cpp
	$(CXX) $(CXXFLAGS) main.cpp rjson.o -o rjson

clean:
	rm -f rjson *.o
