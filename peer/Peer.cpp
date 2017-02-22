/*
 * Peer.cpp
 *
 *  Created on: Feb 16, 2017
 *      Author: Austin
 */

/*
 *	Member function definitions for Peer class
 *	LAST EDITED: 2/20/17
 */

#include <unistd.h>			// included for 'close' function: automatically deallocates.
#include <iostream>			// standard input/output stream
#include <sys/types.h>		// defs of data types used in socket/netinet/etc..
#include <sys/socket.h>		// defs of structs needed for sockets
#include <netinet/in.h>		// constants and structs needed for net domain addresses
#include <cstring>			// for 'memset' as of right now
#include <string>			// for 'to_string'
#include <netdb.h>			// needed for 'netinet/in.h' and addrinfo struct
#include "Peer.hpp"

using namespace std;

int Peer::createSocket(string peerType){
	/*	*Create a socket*
	 *   socketDesc: socket descriptor to return
	 */

	int socketDesc;
	socketDesc = socket(AF_INET, SOCK_STREAM, 0);

	// if peerType == seeder....
	// if Leecher....

	//If socket creation fails
	if(socketDesc == -1){
		cout << "Error with opening socket.\n";
	}
	return socketDesc;

}

int Peer::connectToClient(const char* ipAddr, const char* port){
	/*	*Attempt to Connect to a Client*
	 *   ipAddr: desired IP to connect to
	 *   port: desired port # to connect to
	 *   returns socket descriptor
	 */

	struct addrinfo devInfo, *ptrToDevInfo;

	//set values of addrinfo struct to 0
	memset(&devInfo, 0, sizeof(devInfo));

	devInfo.ai_family = AF_INET;			// or can set to AF_UNSPEC for unspecified
	devInfo.ai_socktype = SOCK_STREAM;		// TCP socket

	// create new socket
	int sockDesc = createSocket("");

	// get desired connection's info
	int connectionStatus = getaddrinfo(ipAddr, port, &devInfo, &ptrToDevInfo);

	// check to make sure information was acquired
	if(connectionStatus != 0){
		cout << "Attempt to get destination info failed.\n";
	}else{

		// attempt connection
		connectionStatus = connect(sockDesc, ptrToDevInfo->ai_addr, ptrToDevInfo->ai_addrlen);

		// check connection
		if(connectionStatus != 0){
			cout << "There was a connection error.\n";
			close(sockDesc);
			sockDesc = -1;
		}
	}
	return sockDesc;

}

int Peer::acceptConnection(int seederDesc){
	/*	*Accept a Peer connection if handshake is valid*
	 * 	 clientInfo: struct w/ info on client socket
	 * 	 sizeOfClientAddr: size of client address
	 */


	struct sockaddr_in clientInfo;
	int clientSocket;
	socklen_t sizeOfClientAddr;
	sizeOfClientAddr = sizeof(clientInfo);

	// attempt to accept connection
	clientSocket = accept(seederDesc, (sockaddr*) &clientInfo, &sizeOfClientAddr);

	// check if connection is valid
	if(clientSocket == -1){
		cout << "Connection to client failed.\n";
	}else{
		/* to do...
		 * .. get peer IP and port
		 * .. verify handshake
		 *
		 */
	}
	return clientSocket;

}

int Peer::bindSocket(const char* ipAddr, int socketDesc){
	// Working on it..
}

// Create a 'Have' message to send to another Peer
string Peer::createHaveMSG(int piece){
	string message = "type:HAVE|piece:" + piece;
	return message;
}

// Create an 'Interested' message to send to another Peer
string Peer::createInterestedMSG(){
	string message = "type:INTERESTED";
	return message;
}

// Return a string with the Peer port and IP numbers
string Peer::peerIPAndPort(struct sockaddr_in &clientInfo){

	char *peerIP = inet_ntoa(clientInfo.sin_addr);		//converts IPv4 address to ASCII string
	int port = ntohs(clientInfo.sin_port);				//converts IP address to host byte order
	return string(peerIP) + ":" + to_string(port);		//to_string will report an error due to a problem w/ eclipse
}

// Change Peer output filename
void Peer::setOutputFileName(const char* name){
	filename = name;
}
