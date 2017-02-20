/*
 * Peer.hpp
 *
 *  Created on: Feb 16, 2017
 *      Author: Austin
 */

#ifndef PEER_HPP_
#define PEER_HPP_
#include <string>
#include <vector>
#include <list>

class Peer{
	//handshakeMsg handshake; **or can this not be a class?
	int numPieces;
	int *bitfield; //vector?
	int *recievedBitfield;
	const char *filename;
	int currentPiece;
	const char* portArray[];	//hardcode valid port numbers?
	int piecesRecieved;
public:
	Peer();

	//Server Specific functions
	int createSocket(std::string peerType);
	int connectToClient(const char* ipAddr, const char* port);
	int acceptConnection(int seederDescriptor);
	int bindSocket(const char* ipAddr, int socketDescriptor);

	//Peer, Seeder, Leecher functions
	bool checkValidHandshake();
	int startSeeding(const char* ipAddr, const char* port);
	int startLeeching(std::list ipPortList);
	void getPeerData(std::list<int> seederList);
	void requestDataFromSeeders(int seederSocket, int pieceToRequest);

	//Hash functions
	std::string getHash(std::string text);
	int checkPieceHash(std::string pieceData, int pieceNum); //check w/server based on piece number
	bool checkHashes(); //what should parameters be..? (recievedHash, serverHash)

	//Message & Piece Functions
	std::string createHaveMSG(int piece);
	std::string createInterestedMSG();
	std::string getBitfieldMSG(std::string message);
	std::string createPieceMSG(int piece, long start, std::string data);
	std::string createBitfieldMSG();
	int createPieceRequest(int index, long start, int length);
	int sendPieceRequest(int socketDescriptor, int index);
	int getPieceMSG(std::string message);
	int getRequestedPiece(std::string message);
	void readSeederMSG(std::string data, int socketDescriptor);
	void readLeecherMSG(std::string data, int socketDescriptor);

	//Not quite sure the best way to do these yet
	std::string getPeerId();	//what to pass?
	std::string getPeerPort();	//^^
};



#endif /* PEER_HPP_ */
