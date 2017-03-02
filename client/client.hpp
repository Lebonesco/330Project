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
	void close_connection();	
private:
	int sock;			//holds socket
	std::string server;		//address of server
	int port;			//predefined port number of server
	struct sockaddr_in sock_addr;	//socket address	


/*
	public:
		String path;
		int sockfd;
		int numBytes;
		char buf[100];
		struct addrinfo hints, *servinfo, *p;
		int rv;
		char s[INET6_ADDRSTRLEN];
		
		Client();

		void get_in_addr(struct sockaddr *sa);
		int connection(int argc, char *argv[]);
		void close_connection();		
	private:
*/

};

#endif /* CLIENT_HPP_ */


