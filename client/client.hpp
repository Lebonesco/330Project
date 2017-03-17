//Client header file
//Created By: Anisha Aggarwal

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <netinet/in.h>
#include "../peer/FunctionalityDemo/Peer.hpp"
#include "../metafile/metafile.hpp"
//#include "../metafile/encoding.cpp"
using namespace std;
using namespace metafile;

class Client {

public:
	Client();
	void getUserData(char &u_or_d);
	bool connection(int argc, char *argv[]);
	bool sendStringData(string data);
	bool sendIntData(int data);
	void receive(int size); 
	string getUploadPath();
	int chooseDownloadFile();
	bool filesComplete(vector<Peer*> p);
	bool checkFileValidity(string path);
	void sendUploadInfo(string path);
	void close_connection();	
	Peer* p;
	vector<Peer*> peers;
	Metafile* m;

private:
	int sock;			//holds socket
	string server;			//address of server
	int port;			//predefined port number of server
	struct sockaddr_in sock_addr;	//socket address	
};

#endif /* CLIENT_HPP_ */


