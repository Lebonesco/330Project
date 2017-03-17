SOURCES = metafile.cpp test.cpp 
HEADERS = metafile.hpp
OBJECTS = $(SOURCES:.cpp=.o)
LIBS = -lm

CXX = g++
CXXFLAGS = -pthread -Wall -Werror -std=c++11 -g

input = metafile.o test.o 

demo: $(input) 
	$(CXX) $(CXXFLAGS) $(input) -o $@ 

metafile.o: metafile.cpp metafile.hpp 
	$(CXX) -c $(CXXFLAGS) metafile.cpp 

test.o: test.cpp
	$(CXX) -c $(CXXFLAGS) test.cpp 

clean:
	$(RM) demo metafile.o test.o                                      
