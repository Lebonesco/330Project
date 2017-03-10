#include <unistd.h>			// included for 'close' function: automatically deallocates.
#include <iostream>			// standard input/output stream
#include <sys/types.h>		// defs of data types used in socket/netinet/etc..
#include <sys/socket.h>		// defs of structs needed for sockets
#include <netinet/in.h>		// constants and structs needed for net domain addresses
#include <arpa/inet.h>		// for inet_ntop
#include <cstring>			// for 'memset' as of right now
#include <string>			// for 'to_string'
#include <sstream>			// for stringstreams
#include <netdb.h>			// needed for 'netinet/in.h' and addrinfo struct
#include <fcntl.h>			// for fcntl()
#include "Peer.hpp"

using namespace std;

Peer::Peer(const int numChunks, vector<string>& recvIpPortList, string type){
	numPieces = numChunks;
	ipPortList = recvIpPortList;
	createBitfield(numChunks, type);

}

int Peer::createSocket(string peerType){
	/*	*Create a socket*
	 *   socketDesc: socket descriptor to return
	 */

	int socketDesc;
	socketDesc = socket(AF_INET, SOCK_STREAM, 0);

	// if peerType == seeder 
	if(peerType == string("SEEDER")){
		fcntl(socketDesc, F_SETFL, O_NONBLOCK);
	}

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
	int connectionStatus = getaddrinfo(ipAddr, port, &devInfo, &ptrToDevInfo);		// populate devInfo

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
	}
	return clientSocket;

}

int Peer::bindAndListenSocket(const char* ipAddr, int socketDesc){
	
	int listener;			
	int newClientSocket;
	int recievedBytes;		// recieved size
	int maxConnectedFD;
	char * socketReadBuff;
	vector<char> socketWriteBuffer;
	fd_set connectedSocketFDs;
	FD_ZERO(&connectedSocketFDs);		// set is initialized to 0
	fd_set allFDs;
	FD_ZERO(&allFDs);
	string serverIP = "127.0.0.1";		// *placeholder of local host for now*
	int port = 8000;

	listener = listen(socketDesc, 5);	// listen for connections; backlog 5

	if(listener != 0){
		cout << "Error listening on selected Port" << endl;
	}
	else{

		FD_SET(socketDesc, &allFDs);	// add socket FD to set of All FD's
		maxConnectedFD = socketDesc;	// set max FD to socket passed in
		cout << "Server ready for connections" << endl;
		
		while(1){		// **need to figure out how/when to terminate this loop**

			connectedSocketFDs = allFDs;

			if(select(maxConnectedFD + 1, &connectedSocketFDs, NULL, NULL, NULL) == -1){	// need to add 1 to max
				cout << "Error selecting File Descriptor (socket)" << endl;	
			}
			else{

				for(int i = 0; i <= maxConnectedFD; i++){
					// check if i is actually a stored connection
					if(FD_ISSET(i, &connectedSocketFDs)){
						recievedBytes = 0;
						
						// if the current socket is the host/server that was passed in
						if(i == socketDesc){
							newClientSocket = acceptConnection(socketDesc);
 
							// if connection didn't fail
							if(newClientSocket != -1){

								FD_SET(newClientSocket, &allFDs);
								if(newClientSocket > maxConnectedFD){
									// set new max FD
									maxConnectedFD = newClientSocket;
									
									// send bitfield to client
									// AS OF NOW this may as well be a place holder since bitfield
									// currently has no functionality!
									send(newClientSocket, bitfield, sizeof(bitfield), 0);
								}
							}

						}else{
							// recieving data from a socket and performing some action

							// MSG_PEEK looks at the next message to be recieved but
							// does not actually read it
							recievedBytes = recv(i, socketReadBuff, 1, MSG_PEEK);
							while(recievedBytes > 0){
								recievedBytes = recv(i, socketReadBuff, 64, 0);
								socketWriteBuffer.insert(socketWriteBuffer.end(), socketReadBuff, socketReadBuff + recievedBytes);
							}
							// copy recieved message data to 'data' variable
							string data = string(socketWriteBuffer.begin(), socketWriteBuffer.end());
							socketWriteBuffer.clear();

							// Take appropriate action based on message type
							readRecvMSG(data, i);

						}
					}
				}

			}
		}	

	}
	return listener;

}

/* TODO:
		1. Handle a recieved bitfield message
			- choose piece and request
		2. handle a piece request
			-  get requested piece and send as piece message
*/
void Peer::readRecvMSG(string data, int socketDescriptor){
	// Handle all cases with one function:
	//		- bitfield message
	//		- piece request

	int bytesSent;

	if(data.find("type:BITFIELDREQ") != string::npos){
		// .... needs to send bitfield to Leecher requesting it
		bytesSent = send(socketDescriptor, bitfield, sizeof(bitfield), 0); 
	}
	if(data.find("type:BITFIELD") != string::npos){
		//update

	}
	if(data.find("type:REQUEST") != string::npos){
		// .... needs to send appropriate piece
		string pieceToSend = "type:PIECE|A placeholder piece";
		bytesSent = send(socketDescriptor, pieceToSend.c_str(), pieceToSend.length(), 0);		// need .c_str() to convert from standard string to c string
	}
	if(data.find("type:PIECE") != string::npos){
		// Write 
	}
	if(data.find("type:UPDATE") != string::npos){

	}

}

// This will be for the client that has the complete file since it will not need
// to request any peices from other clients at any time
int Peer::startSeeding(const char* ipAddr, const char* port){
	// start seeding
	struct addrinfo machineInfo, *ptrToMachineInfo;
	memset(&machineInfo, 0, sizeof(machineInfo));
	int serverStatus;
	int newServerSocket;
	int serverDesc;

	serverStatus = getaddrinfo(ipAddr, port, &machineInfo, &ptrToMachineInfo);
	if(serverStatus == 0){
		newServerSocket = createSocket("SEEDER");
		int s = 1;
		setsockopt(newServerSocket, SOL_SOCKET, SO_REUSEADDR, &s, sizeof(int));		// set socket options so address/port can be reused
		serverStatus = bind(newServerSocket, ptrToMachineInfo->ai_addr, ptrToMachineInfo->ai_addrlen);		// bind socket
		if(serverStatus == 0){
			cout << "Seeder created on port" << port << endl;
		}else{
			cout << "Failed to bind seeder on port" << port << endl;
		}
	}

	if(newServerSocket != 0){
		cout << "Server is ready to listen" << endl;
		bindAndListenSocket(ipAddr, newServerSocket);		// listen and manage connections
	}

	return 0;
}


/* TODO:
		1. Handle forking so peer can act as a seeder while leeching
			- basic attempt implemented (untested)
*/
int Peer::startLeeching(list<string>& ipAndPortList){
	// start leeching
	vector<char> writeBuffer;
	vector<int> seederList;
	int maxLeecherFD;
	int seederSocketFD;
	int i = 0;
	int recievedBytes;
	string ip;
	string port;
	string myPort;		// port for listening
	stirng myIP			// ip for listening
	char* readBuffer = new char[100];		// or whatever size we need it to be
	vector<string>::iterator it;
	fd_set peerFDs;
	FD_ZERO(&peerFDs);

	// MUST FORK THIS FUNCTION
	pid_t pid = fork();
	if(pid == 0){
		startSeeding(myIP, myPort);
	}else if(pid > 0){

		while(!fileComplete()){
			//check for new Seeders
			ipAndPortList = updateIpPortList();

			for(it = ipAndPortList.begin(); it != ipAndPortList.end(); ++it){
				//***FOR NOW*** I'm assuming that the delimeter between port and ip will be a ':'
				
				ip = it->substr(0, it->find(':'));		// get ip		
				port = it->substr(it->find(':') + 1, it->length());		// get port

				seederSocketFD = connectToClient(ip.c_str(), port.c_str());			//connect to seeder: port and ip need to be c strings
				if(!FD_ISSET(seederSocketFD, &peerFDs)){
					FD_SET(seederSocketFD, &peerFDs);
					if(seederSocketFD != -1){
						seederList.push_back(seederSocketFD);			//add seeder to list of seeders
						recievedBytes = recv(seederSocketFD, readBuffer, 1, MSG_PEEK);		// peek at incoming message
						while(recievedBytes > 0){
							memset(readBuffer, 0, 100);			//initialize buffer to 0's
							recievedBytes = recv(seederSocketFD, readBuffer, 100, 0);
							writeBuffer.insert(writeBuffer.end(), readBuffer, readBuffer + recievedBytes);
						}
						string data = string(writeBuffer.begin(), writeBuffer.end());

						//should be new client bitfield every time
						readRecvMSG(data, seederSocketFD);
					}
				}
			}
			if(seederList.size() > 0){
				// need to update list and remove any seeders that no longer have useful data
				// ***still need a function to do this so placeholder for now***
				getPeerData(seederList);
			}
		}
	}else{
		cout << "Fork failed." << endl;
	}
}

/* TODO:
		1. Send update request
		2. Update peer's ipPortList
*/
list<string> Peer::updateIpPortList(){
	// needs to update port/ip from server
}

// Update list of which peers still have interesting data, remove those that don't
void Peer::getPeerData(vector<int> seederList){

	/*
	while(1){
		int x = 0;
		int numFilePieces = 5; 					// placeholder
		for(int j = 0; j < numFilePieces; j++){		// bitfield.length might be syntax error but for now its a placeholder
			if(fileBitfield[bitfield[j]] != 1){			// if peer does not have current piece j
				x = x % seederList.size();
				cout << "Getting piece" << bitfield[j] << endl;
				// send peice request

			}
		}else{

		}
	}
	*/
}

bool Peer::fileComplete(){
	// if all bitfield indices == 1 return true
	for(int i = 0; i < numPieces; i++){
		if(bitfield[i] == 0){
			return false;
		}
	}
	return true;
}

/* TODO: 
 	  1. deal with populating data vector if peer is original seeder
*/
void Peer::createBitfield(int numChunks, string type, string data){
	if(type == "Leech"){
		vector<int> tBitfield (numChunks, 0);
		bitfield = tBitfield;
	}
	if(type == "Seed"){
		vector<int> tBitfield (numChunks, 1);
		bitfield = tBitfield;

		if(data != ""){
			//Split and set file data
		}

	}
}

int main(){
	return 0;
}