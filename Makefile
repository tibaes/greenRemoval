CXXFLAGS = -O3 --std=c++11 -Wall `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`

% : %.cpp
		g++ $(CXXFLAGS) -o $@ $< $(LIBS)
