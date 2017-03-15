#ifndef PEER_HPP_
#define PEER_HPP_
#include <string>
#include <vector>
#include <list>
#include "chunkQueue.hpp"	// rarest piece queue

class Peer{
public:
	int numPieces;
	std::vector<int> bitfield;
	std::vector<const char*> dataBitfield;
	std::vector<std::string> portList;
	const char* selfIP;
	const char* selfPort;
	ChunkQueue* queue;
//public:
	Peer(const int numChunks, std::string port, std::vector<std::string>& ipPortList, std::string type);

	//Server Specific functions
	int createSocket(std::string peerType);								//*
	int connectToClient(const char* ipAddr, const char* port);			//*
	int acceptConnection(int seederDescriptor);
	int bindAndListenSocket(const char* ipAddr, int socketDescriptor);	//*

	//Peer, Seeder, Leecher functions
	int startSeeding(const char* ipAddr, const char* port);
	int startLeeching(std::vector<std::string>& portList);
	void updatePortList(std::string port);
	void getPeerData(std::vector<int> seederList);
	bool fileComplete();
	void createBitfield(int numChunks, std::string type);
	void setFileData(std::vector<const char*> data);

	//Message & Piece Functions									
	void readRecvMSG(std::string data, int socketDescriptor);								//*
	std::string createBitfieldReqMsg();
	std::string createPieceRequest(int index);
	std::string createCompleteMsg();

};



#endif /* PEER_HPP_ */