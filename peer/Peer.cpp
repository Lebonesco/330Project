#include <unistd.h>			// included for 'close' function: automatically deallocates.
#include <iostream>			// standard input/output stream
#include <sys/types.h>		// defs of data types used in socket/netinet/etc..
#include <sys/socket.h>		// defs of structs needed for sockets
#include <netinet/in.h>		// constants and structs needed for net domain addresses
#include <arpa/inet.h>		// for inet_ntop
#include <cstring>			// for 'memset' as of right now
#include <string>			// for 'to_string'
#include <netdb.h>			// needed for 'netinet/in.h' and addrinfo struct
#include <openssl/sha.h>	// include for SHA1 doesn't work yet because I need to configue my build but the function is correct
#include "Peer.hpp"

using namespace std;

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
		
		while(1){

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

}

void Peer::readRecvMsg(string data, int socketDescriptor){
	// Handle all cases with one function:
	//		- bitfield message
	//		- piece request

	int bytesSent;

	if(data.find("type:BITFIELD") != string::npos){
		// .... needs to send bitfield to Leecher requesting it
		int bitfieldToSend [3] = {0, 1, 0};			// *placeholder*
		bytesSent = send(socketDescriptor, bitfieldToSend, sizeof(bitfieldToSend)); 
	}
	if(data.find("type:REQUEST") != string::npos){
		// .... needs to send appropriate piece
		string pieceToSend = "A placeholder";
		bytesSent = send(socketDescriptor, pieceToSend, pieceToSend.length(), 0);
	}
	if(data.find("type:PIECE") != string::npos){
		// Write 
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

int Peer::startLeeching(list<string> ipAndPortList){
	// start leeching
	vector<char> writeBuffer;
	vector<int> seederList;
	int maxLeecherFD;
	int seederSocketFD;
	int i = 0;
	int recievedBytes;
	string ip;
	string port;
	char* readBuffer = new char[100];		// or whatever size we need it to be
	list<string>::iterator it;

	for(it = ipAndPortList.begin(); it != ipAndPortList.end(); ++it){
		//***FOR NOW*** I'm assuming that the delimeter between port and ip will be a ':'
		
		ip = it->substr(0, it->find(':'));		// get ip		
		port = it->substr(it->find(':') + 1, it->length());		// get port

		seederSocketFD = connectToClient(ip, port);			//connect to seeder
		if(seederSocketFD != -1){
			seederList.push_back(seederSocketFD);			//add seeder to list of seeders
			recievedBytes = recv(seederSocketFD, readBuffer, 1, MSG_PEEK);		// peek at incoming message
			while(recievedBytes > 0){
				memset(readBuffer, '-1', 100);			//initialize buffer to -1's
				recievedBytes = recv(seederSocketFD, readBuffer, 100, 0);
				writeBuffer.insert(writeBuffer.end(), readBuffer, readBuffer + recievedBytes);
			}
			string data = string(writeBuffer.begin(), writeBuffer.end());
			readRecvMsg(data, seederSocketFD);
		}
	}
	if(seederList.size() > 0){
		// need to update list and remove any seeders that no longer have useful data
		// ***still need a function to do this so placeholder for now***
		updatePeersOfInterest(seederList);
	}
}

// Update list of which peers still have interesting data, remove those that don't
void Peer::updatePeersOfInterest(vector<int> seederList){
	
}






int Peer::createPieceRequest(int index, long start, int length){
	/*	*Create message to request a piece based on returned Peer bitfield*
	 *
	 */

	stringstream ss;

	ss << "type:REQUEST";
	ss << "|index:" << index;
	ss << "|start:" << start;
	ss << "|length:" << length;
	return ss.str();
}

// this might also be able to be done when piece request is received
string Peer::createPieceMSG(int piece, long start, string data){
	//Creates a piece message to send to a leecher

	stringstream ss;

	ss << "type:PIECE";
	ss << "|index:" << piece;
	ss << "|start:" << start;
	ss << "|data:" << data;

	return ss.str();
}

// ** leaving hash functionality until later **
string Peer::createHash(string text){
	//	create the SHA-1 hash for a piece of text

	char hash[21];
	char output[41];
	memset(hash, '\0', 21);
	memset(output, '\0', 41);

	SHA1((unsigned char*) text.c_str(), text.length(), (unsigned char*) hash);		//doesn't currently work b/c of include issue
	for(int i = 0; i < 20; i++){
		sprintf(output + i * 2, "02x", (unsigned int) hash[i]);
	}
	return string(output);		//returns hash of 'text'
}

string Peer::peerIPAndPort(struct sockaddr_in &clientInfo){
	//	*Return a string with the Peer port and IP numbers*

	char *peerIP = inet_ntoa(clientInfo.sin_addr);		//converts IPv4 address to ASCII string
	int port = ntohs(clientInfo.sin_port);				//converts IP address to host byte order
	return string(peerIP) + ":" + to_string(port);		//to_string will report an error due to a problem w/ eclipse
}

void Peer::setOutputFileName(const char* name){
	// Change Peer output filename

	filename = name;
}