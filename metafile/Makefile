SOURCES = metafile.cpp main.cpp bencode.c
HEADERS = metafile.hpp bencode.h list.h
OBJECTS = $(SOURCES:.cpp=.o) $(SOURCES:.c=.o)
LIBS = -lm

CC = gcc
CXX = g++
CFLAGS = -std=c99 -g
CXXFLAGS = -Wall -Werror -std=c++11 -g

input = metafile.o main.o bencode.o

metafile: $(input) 
	$(CXX) $(CXXFLAGS) $(input) -o $@ 

metafile.o: metafile.cpp metafile.hpp
	$(CXX) -c $(CXXFLAGS) metafile.cpp

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp metafile.cpp

bencode.o: bencode.c bencode.h list.h
	$(CC) -c $(CCFLAGS) bencode.c


clean:
	$(RM) metafile metafile.o main.o bencode.o                                         
