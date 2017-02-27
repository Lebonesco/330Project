//Client header file

#ifndef CLIENT_HPP_
#define CLIENT_HPP_

#include <iostream>
#include <string>
#include <vector>
#include <list>


class Client {

	public:
		string path;
		int sockfd;
		int numBytes;
		char buf[100];
		struct addrinfo hints, *servinfo, *p;
		int rv;
		char s[INET6_ADDRSTRLEN];
		
		Client();

		void get_in_address(struct sockaddr *sa);
		void connection_made();
		void close_connection();		
	private:


};

#endif /* CLIENT_HPP_ */


