SOURCES = metafile.cpp main.cpp bencode.c
HEADERS = metafile.hpp bencode.h list.h
OBJECTS = $(SOURCES:.cpp=.o) $(SOURCES:.c=.o)
LIBS = -lm

CC = gcc
CXX = g++
CFLAGS = -Wall -c
CXXFLAGS = -std=c++11 -g

metafile: $(OBJECTS)

main.o: main.cpp main.hpp
	$(CXX) $(CXXFLAGS) main.cpp

metafile.o: metafile.cpp metafile.hpp
	$(CXX) $(CXXFLAGS) metafile.cpp

bencode.o: bencode.c bencode.h list.h
	$(CC) $(CCFLAGS) bencode.c


clean:
	$(RM) metafile metafile.o main.o bencode.o                                         
