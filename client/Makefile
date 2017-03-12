SOURCES = client_interface.cpp main.cpp
HEADERS = client.hpp
OBJECTS = $(SOURCES:.cpp=.o)

RM = rm
CC = g++
CPPFLAGS = -std=c++11

%.o: %.c
	$(CC) -c $(CPPFLAGS) $<

all: client

client: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

clean:
	$(RM) client $(OBJECTS)
