//Client header file
//Created By: Anisha Aggarwal

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <netinet/in.h>

class Client {

public:
	Client();
	bool connection(int argc, char *argv[]);
	bool sendStringData(std::string data);
	bool sendIntData(int data);
	std::string receive(int size); 
	std::string getUploadPath();
	int chooseDownloadFile();
	bool checkFileValidity(std::string path);
	void sendUploadInfo(std::string path);
	void close_connection();	
private:
	int sock;			//holds socket
	std::string server;		//address of server
	int port;			//predefined port number of server
	struct sockaddr_in sock_addr;	//socket address	

};

#endif /* CLIENT_HPP_ */


