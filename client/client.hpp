//Client header file

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
	bool send_data(std::string data);
	std::string receive(int size); 
	std::string getUploadPath();
	int chooseDownloadFile();
	long getFileSize(std::string path);
	void sendUploadFile(std::string path);
	void close_connection();	
private:
	int sock;			//holds socket
	std::string server;		//address of server
	int port;			//predefined port number of server
	struct sockaddr_in sock_addr;	//socket address	

};

#endif /* CLIENT_HPP_ */


