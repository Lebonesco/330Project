/*
 * Peer.cpp
 *
 *  Created on: Feb 16, 2017
 *      Author: Austin
 */

/*
 *	Member function definitions for Peer class
 *	LAST EDITED: 2/22/17
 */

#include <unistd.h>			// included for 'close' function: automatically deallocates.
#include <iostream>			// standard input/output stream
#include <sys/types.h>		// defs of data types used in socket/netinet/etc..
#include <sys/socket.h>		// defs of structs needed for sockets
#include <netinet/in.h>		// constants and structs needed for net domain addresses
#include <arpa/inet.h>		// for inet_ntop
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

	if(socketDesc == -1){			//If socket creation fails
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

	memset(&devInfo, 0, sizeof(devInfo));	//set values of addrinfo struct to 0

	devInfo.ai_family = AF_INET;			// or can set to AF_UNSPEC for unspecified
	devInfo.ai_socktype = SOCK_STREAM;		// TCP socket

	int sockDesc = createSocket("");		// create new socket
	int connectionStatus = getaddrinfo(ipAddr, port, &devInfo, &ptrToDevInfo);		// get desired connection's info

	if(connectionStatus != 0){				// check to make sure information was acquired
		cout << "Attempt to get destination info failed.\n";
	}else{

		connectionStatus = connect(sockDesc, ptrToDevInfo->ai_addr, ptrToDevInfo->ai_addrlen);		// attempt connection

		if(connectionStatus != 0){			// check connection
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

	clientSocket = accept(seederDesc, (sockaddr*) &clientInfo, &sizeOfClientAddr); // attempt to accept connection
	if(clientSocket == -1){							// check if connection is valid
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

int Peer::bindAndListenSocket(const char* ipAddr, int socketDesc){
	/*	*Bind to socket and Listen for connections*
	 * 	 status: current status of listening
	 * 	 listener: socket descriptor of socket to listen for
	 */
	int status, listener;

	listener = socketDesc;				//assign listener socket
	status = listen(listener, 10);		//listen for incoming connection w/backlog up to 10

	/*
	  newConnFD: variable to hold file descriptor for new connection
	  clientAddr: struct to hold client address data
	  addrSize: size of address
	  s: array to hold IPv6 address, (INET6... = 46: length of IPv6 address)
	 */

	int newConnFD;
	struct sockaddr_storage clientAddr;
	socklen_t addrSize;
	char s[INET6_ADDRSTRLEN];

	addrSize = sizeof(clientAddr);		// set address size to size of client address struct

	while(1){							//loop for connections
		newConnFD = accept(listener, (sockaddr*) &clientAddr, &addrSize);		//accept connection request and assign connection info to connection file descriptor variable
		if(newConnFD < 0){
			cout << "Error.";
			continue;
		}

		//converts internet network address to string in internet standard format
		inet_ntop(clientAddr.ss_family, (sockaddr*) &clientAddr, s, sizeof(s));		// might need helper function to return correct val for IPv4 or IPV6

		status = send(newConnFD, "Connected", 9, 0);		//send message to connected peer

		if(status == -1){									//close if status code is -1
			close(newConnFD);
			_exit(3);
		}
	}
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
