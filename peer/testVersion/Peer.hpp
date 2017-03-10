#ifndef PEER_HPP_
#define PEER_HPP_
#include <string>
#include <vector>
#include <list>

class Peer{
	int numPieces;
	std::vector<int> bitfield;
	std::vector<std::string> dataBitfield;
	std::vector<std::string> ipPortList;
	std::string selfIP;
public:
	Peer(const int numChunks, std::string iP, std::vector<std::string>& ipPortList, std::string type);

	//Server Specific functions
	int createSocket(std::string peerType);								//*
	int connectToClient(const char* ipAddr, const char* port);			//*
	int acceptConnection(int seederDescriptor);
	int bindAndListenSocket(const char* ipAddr, int socketDescriptor);	//*

	//Peer, Seeder, Leecher functions
	int startSeeding(const char* ipAddr, const char* port);
	int startLeeching(std::vector<std::string>& ipAndPortList);
	std::vector<std::string> updateIpPortList();
	void getPeerData(std::vector<int> seederList);
	bool fileComplete();
	void createBitfield(int numChunks, std::string type, std::string data = "");

	//Hash functions
	std::string createHash(std::string text);
	int checkPieceHash(std::string pieceData, int pieceNum); //check w/server based on piece number
	bool checkHashes(); //what should parameters be..? (recievedHash, serverHash)

	//Message & Piece Functions									
	void readRecvMSG(std::string data, int socketDescriptor);								//*

};



#endif /* PEER_HPP_ */