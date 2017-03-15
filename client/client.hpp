//Client header file
//Created By: Anisha Aggarwal

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <netinet/in.h>
using namespace std;

class Client {

public:
	Client();
	bool connection(int argc, char *argv[]);
	bool sendStringData(string data);
	bool sendIntData(int data);
	string receive(int size); 
	string receive(string s); 
	string getUploadPath();
	int chooseDownloadFile();
	bool checkFileValidity(string path);
	void sendUploadInfo(string path);
	void close_connection();	
private:
	int sock;			//holds socket
	string server;			//address of server
	int port;			//predefined port number of server
	struct sockaddr_in sock_addr;	//socket address	
};

#endif /* CLIENT_HPP_ */


