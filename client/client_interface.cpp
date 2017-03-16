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
#include <vector>
#include "client.hpp"
using namespace std;
//using namespace metafile;
//using namespace peer;

#define PORT 8500
#define MAXDATASIZE 100

//figure out what the user wants to do
void Client::getUserData(char &u_or_d) {

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
bool Client::sendStringData(string data) {
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
string Client::receive(int size) {
	char buffer[size];
	std::string s_reply = "";
	
	if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
		cout << "Receive has failed" << endl;
	}

	s_reply = buffer;
	cout << "Server sent: " << s_reply << endl;
	return s_reply;
}

//receive string from server
string Client::receive(string s) {
	char buffer[100];
	string s_reply = s;
	
	if (recv(sock, buffer, sizeof(buffer), 0) < 0) {
		cout << "Receive has failed" << endl;
	}

	cout << "Server sent: " << s_reply << endl;

	return s_reply;
}


//get user input on path of file to upload
//open file and send it to server
string Client::getUploadPath() {
	string path;

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
bool Client::checkFileValidity(string path) {

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

void Client::sendUploadInfo(string path){

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
string Client::encode(int x, std::string path) {
	string r;
	r.append(to_string(x));
	r.append(":");
	r.append(path);
	return r;
}

//encode string
bool Client::filesComplete(vector<Peer*> peerList) {
	bool complete;

	//iterate through the peerList
	//check if files are complete
	for (int i = 0; i < peerList.size(); ++i) {
		if (complete == true) {
			cout << peerList[i] << endl;
			complete = peerList[i]->fileComplete();		
		} else {
			return complete;
		}
	}

	return complete;
}

//close connection to server
void Client::close_connection() {
	close(sock);
}

