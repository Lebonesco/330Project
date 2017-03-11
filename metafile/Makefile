SOURCES = metafile.cpp main.cpp 
HEADERS = metafile.hpp
OBJECTS = $(SOURCES:.cpp=.o) $(SOURCES:.c=.o)
LIBS = -lm

CC = gcc
CXX = g++
CCFLAGS = -std=c99 -g
CXXFLAGS = -Wall -Werror -std=c++11 -g

input = metafile.o main.o 

metafile: $(input) 
	$(CXX) $(CXXFLAGS) $(input) -o $@ 

metafile.o: metafile.cpp metafile.hpp 
	$(CXX) -c $(CXXFLAGS) metafile.cpp 

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp metafile.cpp

clean:
	$(RM) metafile metafile.o main.o                                      
