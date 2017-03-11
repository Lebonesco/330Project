SOURCES = Peer.cpp
HEADERS = Peer.hpp
OBJECTS = $(SOURCES:.cpp=.o)
LIBS = -lm

CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11 -g

%.o: %.cpp
	$(CXX) -c $(CXXFLAG) $<

main: $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS)

clean:
	$(RM) main $(OBJECTS)