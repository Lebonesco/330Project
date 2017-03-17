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
#include <set>				// for the magic that is sets
#include "Peer.hpp"
#include "chunkQueue.hpp"
//#include "metafile.hpp"

using namespace std;

Peer::Peer(const int numChunks, string port, vector<string>& recvPortList, string type){
	numPieces = numChunks;
	portList = recvPortList;
	queue = new ChunkQueue(numChunks);
	createBitfield(numChunks, type);
	selfPort = port.c_str();
	string iP = "127.0.0.1";
	selfIP = iP.c_str();

	if(type == "Leech"){
		vector<const char*> dataBF (numChunks);
		dataBitfield = dataBF;
	}

}

int Peer::createSocket(string peerType){
	/*	*Create a socket*
	 *   socketDesc: socket descriptor to return
	 */

	int socketDesc;
	socketDesc = socket(AF_INET, SOCK_STREAM, 0);

	// if peerType == seeder 
	if(peerType == string("Seeder")){
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
		cout << "function:connectToClient -> No error connecting to client." << endl;


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
	char socketReadBuff[100];
	vector<char> socketWriteBuffer;
	fd_set connectedSocketFDs;
	FD_ZERO(&connectedSocketFDs);		// set is initialized to 0
	fd_set allFDs;
	FD_ZERO(&allFDs);
	string serverIP = "127.0.0.1";		// *placeholder of local host for now*
	pid_t pid = fork();

	if(pid == 0){
		listener = listen(socketDesc, 5);	// listen for connections; backlog 5
	}else if(pid > 0){

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
								cout << "(Server): Connection accepted." << endl;
	 
								// if connection didn't fail
								if(newClientSocket != -1){

									FD_SET(newClientSocket, &allFDs);
									cout << "(Server): FD added to connection set." << endl;
									if(newClientSocket > maxConnectedFD){
										// set new max FD
										maxConnectedFD = newClientSocket;
									}
								}

							}else{
								// recieving data from a socket and performing some action

								// MSG_PEEK looks at the next message to be recieved but
								// does not actually read it
								recievedBytes = recv(i, socketReadBuff, 1, MSG_PEEK);
	//							cout << "(Server): Message Recieved." << endl;
								while(recievedBytes > 0){
									recievedBytes = recv(i, socketReadBuff, sizeof(socketReadBuff), 0);
									socketWriteBuffer.insert(socketWriteBuffer.end(), socketReadBuff, socketReadBuff + recievedBytes);
									if(recievedBytes < 64){
										break;
									}
								}
								// copy recieved message data to 'data' variable
								string data = string(socketWriteBuffer.begin(), socketWriteBuffer.end());
								socketWriteBuffer.clear();

								// Take appropriate action based on message type
								// and check if client has sent a 'completed' message
	//							cout << "(Server): Reading message: " << data << endl;
								readRecvMSG(data, i);

								// ***Completed message type doesn't currently exist...***
								if(data.find("type:COMPLETED") != string::npos){
									cout << "File transfer to peer completed. Closing connection." << endl;
									close(i);
									FD_CLR(i, &allFDs);
								}
							}
						}
					}

				}
			}	

		}
		return listener;
	}else{
		cout << "bindAndListen fork failed." << endl;
	}

}

/* TODO:
		1. Handle a recieved bitfield message
			- choose piece and request
		2. handle a piece request
			-  get requested piece and send as piece message
		3. bencoding (encode/decode)
*/
void Peer::readRecvMSG(string data, int socketDescriptor){
	
	int bytesSent;

	if(data.find("type:REQBITFIELD") != string::npos){
		// .... needs to send bitfield to Leecher requesting it
		cout << "RECIEVED: Bitfield Request." << endl;
		
		string bitfieldMsgToSend = "type:BITFIELD";
		stringstream ss;
		string bfString;

		for(size_t i = 0; i < bitfield.size(); ++i){
			ss << bitfield[i];
		}
		bfString = ss.str();
		bitfieldMsgToSend += bfString;

		bytesSent = send(socketDescriptor, bitfieldMsgToSend.c_str(), bitfieldMsgToSend.length(), 0); 
		cout << "SENT: " << bytesSent << " bytes. Message: " << bitfieldMsgToSend << endl;
	}

	if(data.find("type:BITFIELD") != string::npos){
		// mark indices where data is available
		cout << "RECIEVED: Bitfield." << endl;

		string recBF = data.substr(13);
		vector<int> recBitfield (numPieces, 0);

		for(int i = 0; i < recBF.size(); i++){
			recBitfield[i] = (int)recBF[i];
		}
		cout << "Recieved Bitfield: ";
		for(int j = 0; j < recBF.size(); j++){
			cout << recBF[j] << " ";
		}
		cout << endl;
		queue->updateQueue(bitfield, recBitfield);
	}

	if(data.find("type:REQPIECE") != string::npos){
		// .... needs to send appropriate piece
		const char* dataToSend;
		int index = (int)data[13] - 48;
		cout << "RECIEVED: Piece Request." << endl;
		dataToSend = dataBitfield[index];

		string pieceToSend = "type:PIECE|index:1A placeholder piece";
		bytesSent = send(socketDescriptor, pieceToSend.c_str(), pieceToSend.length(), 0);		// need .c_str() to convert from standard string to c string
		cout << "SENT: " << bytesSent << " bytes. Message: " << pieceToSend << endl;
	}

	if(data.find("type:PIECE") != string::npos){
		// Write 
		cout << "RECIEVED: Data Piece." << endl;

		//char sIndex = data.at(data.find("index:"));
		char sIndex = data[17];
		cout << "index: " << sIndex << endl;
		int index = (int)sIndex - 48;
		cout << "RECIEVED PIECE. Index: " << index << endl;

		string dataToWrite = data.substr(data.find("type:PIECE|index:") + 1);
		dataBitfield[index] = dataToWrite.c_str();
		bitfield[index] = 1;		// update peer's bitfield
	}

	if(data.find("FILE TRANSFER TO PEER COMPLETE") != string::npos){
		::close(socketDescriptor);
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
		serverStatus = ::bind(newServerSocket, ptrToMachineInfo->ai_addr, ptrToMachineInfo->ai_addrlen);		// bind socket
		if(serverStatus == 0){
			cout << "Seeder created on port " << port << endl;
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



int Peer::startLeeching(vector<string>& currentPortList){
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
	string myIP;			// ip for listening
	char readBuffer[100];		// or whatever size we need it to be	
	fd_set peerFDs;
	FD_ZERO(&peerFDs);

	// MUST FORK THIS FUNCTION
	
	pid_t pid = fork();
	if(pid == 0){
		startSeeding(selfIP, selfPort);
	}else if(pid > 0){
	

		while(!fileComplete()){

			for(i = 0; i < portList.size(); i++){
						
				port = portList[i];		// get port

				if(port == selfPort){
					continue;
				}else{
					cout << "Attempting to connect to port " << portList[i] << endl;

					seederSocketFD = connectToClient(selfIP, port.c_str());			//connect to seeder: port and ip need to be c strings
					if(seederSocketFD != -1){
						if(!FD_ISSET(seederSocketFD, &peerFDs)){
							FD_SET(seederSocketFD, &peerFDs);
							cout << "(Client): Added new FD to set." << endl;
							if(seederSocketFD != -1){
								seederList.push_back(seederSocketFD);			//add seeder to list of seeders
								string bFReqMsg = createBitfieldReqMsg();
								send(seederSocketFD, bFReqMsg.c_str(), bFReqMsg.length(), 0);
								cout << "(Client): Sent bfReq -> " << bFReqMsg << endl;
								recievedBytes = recv(seederSocketFD, readBuffer, sizeof(readBuffer), MSG_PEEK);		// peek at incoming message
								while(recievedBytes > 0){
									// decode message before passing to readRecvMSG
									memset(readBuffer, 0, 100);			//initialize buffer to 0's
									cout << "(Client): Waiting for response." << endl;
									recievedBytes = recv(seederSocketFD, readBuffer, sizeof(readBuffer), 0);
									cout << "(Client): Response received. ";
									cout << "Bytes Received: " << recievedBytes << endl;
									writeBuffer.insert(writeBuffer.end(), readBuffer, readBuffer + recievedBytes);
									string data = string(writeBuffer.begin(), writeBuffer.end());
									cout << "Message Recieved: " << data << endl;
									if(recievedBytes < 100){
										break;
									}
								}
								string data = string(writeBuffer.begin(), writeBuffer.end());
								writeBuffer.clear();
								cout << "Message from Server: " << data << endl;

								//should be new client bitfield every time
								cout << "(Client): Reading message -> " << data << endl;
								readRecvMSG(data, seederSocketFD);
							}
						}
					}else{
						continue;
					}
				}
			}
			if(seederList.size() > 0){
				cout << "(Client): Getting data from connected Peers." << endl;
				for(int i = 0; i < seederList.size(); i++){
					string bFReqMsg = createBitfieldReqMsg();
					send(seederList[i], bFReqMsg.c_str(), bFReqMsg.length(), 0);
					recievedBytes = recv(seederSocketFD, readBuffer, sizeof(readBuffer), MSG_PEEK);		// peek at incoming message
					while(recievedBytes > 0){
						// decode message before passing to readRecvMSG
						memset(readBuffer, 0, 100);			//initialize buffer to 0's
						recievedBytes = recv(seederSocketFD, readBuffer, sizeof(readBuffer), 0);
						writeBuffer.insert(writeBuffer.end(), readBuffer, readBuffer + recievedBytes);
						string data1 = string(writeBuffer.begin(), writeBuffer.end());
						if(recievedBytes < 100){
							break;
						}
					}
					string data1 = string(writeBuffer.begin(), writeBuffer.end());
					writeBuffer.clear();

					vector<int> recBitfield;
					string bf = data1.substr(12);
					set<int> available;
					int choosenIndex;
					// populate current connections available bitfield
					for(int j = 0; j < bf.length(); j++){
						recBitfield.push_back((int)bf[j] - 48);
					}
					// populate available set
					for(int k = 0; k < recBitfield.size(); k++){
						if(recBitfield[k] == 1){
							available.insert(k);
						}else{
							continue;
						}
					}

					choosenIndex = queue->getChunk(available);
					cout << "Requesting piece at index: " << choosenIndex << endl;

					string pRMsg = createPieceRequest(choosenIndex);
					cout << "(Client): Sending Message -> " << pRMsg << endl;
					send(seederList[i], pRMsg.c_str(), pRMsg.length(), 0);
					recievedBytes = recv(seederList[i], readBuffer, sizeof(readBuffer), MSG_PEEK);
					while(recievedBytes > 0){
						recievedBytes = recv(seederList[i], readBuffer, sizeof(readBuffer), 0);
						cout << "(Client): Response received." << endl;
						cout << "Bytes Received: " << recievedBytes << endl;
						writeBuffer.insert(writeBuffer.end(), readBuffer, readBuffer + recievedBytes);
						string data = string(writeBuffer.begin(), writeBuffer.end());
						cout << "Message Recieved: " << data << endl;
						if(recievedBytes < 100){
							break;
						}
					}
					string data = string(writeBuffer.begin(), writeBuffer.end());
					writeBuffer.clear();
					cout << "Message from Server: " << data << endl;
					readRecvMSG(data, seederList[i]);
				}
			}
		}
		//SEND FILE COMPLETE MESSAGE
		string fCMsg = createCompleteMsg();
		for(int i = 0; i < seederList.size(); i++){
			send(seederList[i], fCMsg.c_str(), fCMsg.length(), 0);
		}
	}else{
		cout << "Fork failed." << endl;
	}
	return 0;
}

/* TODO:
		1. Send update request
		2. Update peer's ipPortList
*/
void Peer::updatePortList(vector<string>& ports){
	// needs to update port/ip from server
	portList = ports;
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

void Peer::createBitfield(int numChunks, string type){
	if(type == "Leech"){
		vector<int> tBitfield (numChunks, 0);
		bitfield = tBitfield;
	}
	if(type == "Seed"){
		vector<int> tBitfield (numChunks, 1);
		bitfield = tBitfield;
	}
}


void Peer::setFileData(vector<char*>&  data){

	for(int i = 0; i < data.size(); i++){
		dataBitfield.push_back((const char*)data[i]);
	}
}


string Peer::createBitfieldReqMsg(){
	return "type:REQBITFIELD";
}

string Peer::createPieceRequest(int index){
	stringstream ss;
	string msg;

	ss << "type:REQPIECE";
	ss << index;
	msg = ss.str();

	return msg;
}

string Peer::createCompleteMsg(){
	return "FILE TRANSFER TO PEER COMPLETE";
}

/*
int main(){

	int chunks1 = 5;
	int chunks2 = 5;
	string testIP = "127.0.0.1";
	vector<string> testList;
	string testPort1 = "8000";
	string testPort2 = "8080";
	testList.push_back(testPort1);
	testList.push_back(testPort2);

	Peer* seeder = new Peer(chunks1, testPort1, testList, "Leech");
	Peer* leecher = new Peer(chunks2, testPort2, testList, "Leech");

	//cout << seeder->selfIP << endl;
	//cout << seeder->portList[0];


	seeder->startSeeding(seeder->selfIP, seeder->selfPort);
//	leecher->startLeeching(leecher->portList);

	return 0;
}
*/