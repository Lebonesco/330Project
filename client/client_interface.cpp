//Client interface to be able to connect to server/other peers
//Created by: Anisha Aggarwal

#include <iostream>
#include <cstring>
#include <string>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <typeinfo>
#include <map>
#include "client.hpp"
using namespace std;
//using namespace metafile;
//using namespace peer;

#define PORT 8500
#define MAXDATASIZE 100

//figure out what the user wants to do
void getUserData(char &u_or_d) {

	//check to make sure user entered either upload or download
	while ((u_or_d != 'u') && (u_or_d != 'd')) {
		cout << "Would you like to upload(u) or download(d) a file? ";
        	cin >> &u_or_d;
        	cout << "Upload or Download: " << u_or_d << endl;
	}
	
}

//client constructor
Client::Client() {
	port = PORT;
	server = "127.0.0.1";	//server is localhost
	sock = -1;
}

//check if connection was successful
bool Client::connection(int argc, char *argv[]) {
	
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == -1) {
		cout << "Error creating socket" << endl;
		return false;
	}
	cout << "Socket successful" << endl;
	
	if(inet_addr(server.c_str()) != -1) {
		//specified address is in IP format
		sock_addr.sin_addr.s_addr = inet_addr(server.c_str());
		cout << "specified address in IP format" << endl;
	} else {
		//specified address is a host name
		//resolve it to an IP address
		struct hostent *h;
		struct in_addr **addr_list;

		if ((h = gethostbyname(server.c_str())) == NULL) {
			cout << "gethostbyname failed" << endl;
			return false;
		}
		cout << "gethostname is " << server.c_str() << endl;
		addr_list = (struct in_addr **) h->h_addr_list;
		for (int i = 0; addr_list[i]; i++) {
			sock_addr.sin_addr = *addr_list[i];
		}
	}

	sock_addr.sin_family = AF_INET;
	sock_addr.sin_port = htons(port);

	//connect to server
	if (connect(sock, (struct sockaddr *)&sock_addr, sizeof(sock_addr)) < 0) {
		cout << "connection has failed" << endl;
		return false;
	}
	cout << "connection successful" << endl;
	return true;
	
}

//send data to server
bool Client::sendStringData(std::string data) {
	if (send(sock, data.c_str(), strlen(data.c_str()), 0) < 0) {
		cout << "Send has failed" << endl;
		return false;
	}
	cout << data << " sent" << endl;

	return true;
}

/*
//send int data to server
bool Client::sendIntData(int data) {
        if (send(sock, data, 100, 0) < 0) {
                cout << "Send has failed" << endl;
                return false;
        }
        cout << data << " sent" << endl;

        return true;
}
*/

//receive data from server
std::string Client::receive(int size) {
	char buffer[size];
	std::string s_reply = "";
	
	if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
		cout << "Receive has failed" << endl;
	}

	s_reply = buffer;
	return s_reply;
}

//receive string from server
std::string Client::receive(std::string s) {
	char buffer[100];
	std::string s_reply = s;
	
	if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
		cout << "Receive has failed" << endl;
	}

	return s_reply;
}


//get user input on path of file to upload
//open file and send it to server
std::string Client::getUploadPath() {
	std::string path;

	cout << "Please enter the path to the file you would like to upload: ";
	cin >> path;
	cout << '\n';

	cout << "Path: " << path << endl;

	//look for file on thier system
	ifstream file(path.c_str());

	while (!file) {
		cout << "Error while opening the file" << endl;
		
		cout << "Reenter the path to the file: ";
		cin >> path;
        	cout << '\n';

       		ifstream file(path.c_str());
	}
	
	return path;
}

/*
bool Client::checkFileValidity(std::string path) {

	FILE *upload_file;
	//unsigned long file_size;
	bool valid = false;

	upload_file =  fopen(path.c_str(), "r");
	
	if (upload_file == NULL) {
		valid = false;
		cout << "File not found" << endl;
	} else {
		valid = true;
		cout << "File " << upload_file << " found" << endl;
	}

	if (valid == true) {
		//send server message with file name, and file size
		//send();	
	}
	fclose(upload_file);
	return valid;
}
*/

void Client::sendUploadInfo(std::string path){

	FILE *upload_file;

	upload_file = fopen(path.c_str(), "r");	
	
	cout << "Done sending file" << endl;
	fclose(upload_file);

}

//get user input on what file they would like to download
int Client::chooseDownloadFile() {
	int fileNum;

	cout << "Enter number of file would you like to download: ";
	cin >> fileNum;
	cout << "File number entered: " << fileNum << endl;
	
	return fileNum;
}

//encode integer
string encode(int x) {
	string r;
	r.append("i");
	r.append(to_string(x));
	r.append("e");
	return r;
}

//encode string
string encode(string x) {
	string r;
	int length = x.length();
	r.append(to_string(length));
	r.append(":");
	r.append(x);
	return r;
}
/*
bool fileComplete(std::vector<Peer* p> peerList) {
	int port_it = 9000;

	//iterate through the peerList
	//check if files are complete
	//for ( 

	return true;
}
*/
//close connection to server
void Client::close_connection() {
	close(sock);
}

/*
int main(int argc, char *argv[]) {

	Client c;
	
	int size = 512;
	std::string message;
	std::string path;
	std::string port;
	std::vector<std::string> ports;
	std::vector<peer::Peer* p> peers;
	bool connected = false;	
	std::string peer_info;
	std::string bencoded_info;


		//send client message that user wants to download
		message = "download";
		c.sendStringData(message);

		//display the options of files to be downloaded
		//get user input on which file they would like to download
		//c.chooseDownloadFile();
		bencoded_info = c.receive(size);
		//peer_info = decode(bencoded_info);
		cout << bencoded_info << endl;
		//server sends message of number of packages to be sent
		//recieve listing from server of downloadable files
		cout << c.receive(size) << endl;
	
		//peer class starts leeching
		Peer* leecher1(m.chunkNumber, 9001, ports, "leecher");
		peers.pushback(leecher1);
		Peer* leecher2(m.chunkNumber, 9002, ports, "leecher");
		peers.pushback(leecher2);
		Peer* leecher3(m.chunkNumber, 9003, ports, "leecher");
		peers.pushback(leecher3);
		for (std::vector<Peer* p>::iterator it = peers.begin(); it != peers.end(); ++it) {
			cout << it << endl;
		}

		//constantly listen for update list from server to send to peer
		while(!filesComplete) {
			//iterate through peers vector
			//update peer list
			
		}
	}
	c.close_connection();

	return 0;	
}
*/
