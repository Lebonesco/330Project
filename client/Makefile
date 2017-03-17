SOURCES =  main.cpp client_interface.cpp ../metafile/metafile.cpp ../peer/FunctionalityDemo/chunkQueue.cpp ../peer/FunctionalityDemo/Peer.cpp 
HEADERS = client.hpp ../peer/FunctionalityDemo/Peer.hpp ../peer/FunctionalityDemo/chunkQueue.hpp ../metafile/metafile.hpp
OBJECTS = $(SOURCES:.cpp=.o)

RM = rm
CC = g++
CPPFLAGS = -std=c++11

%.o: %.c
	$(CC) -c $(CPPFLAGS) $<

//all: client

client: $(OBJECTS)
	$(CC) -o $@ $(OBJECTS)

clean:
	$(RM) client $(OBJECTS)
