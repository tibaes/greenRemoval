CXX = g++
CXXFLAGS = -O3 --std=c++11 -Wall `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

all: greenRemoval.out

%.out: %.cpp
		$(CXX) $(CXXFLAGS) -o $@ $< $(LIBS)
