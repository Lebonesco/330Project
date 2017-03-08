//Client interface to be able to connect to server/other peers

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
#include "client.hpp"
using namespace std;

#define PORT 9000
#define MAXDATASIZE 100

//figure out what the user wants to do
void getUserData(char &u_or_d) {

	//check to make sure user entered either upload or download
	while ((u_or_d != 'u') || (u_or_d != 'd')) {
		cout << "Would you like to upload(u) or download(d) a file? ";
        	cin >> u_or_d;
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
	} else {
		//specified address is a host name
		//resolve it to an IP address
		struct hostent *h;
		struct in_addr **addr_list;

		if ((h = gethostbyname(server.c_str())) == NULL) {
			cout << "gethostbyname failed" << endl;
			return false;
		}
		
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
bool Client::send_data(std::string data) {
	if (send(sock, data.c_str(), strlen(data.c_str()), 0) < 0) {
		cout << "Send has failed" << endl;
		return false;
	}
	cout << "Data sent" << endl;

	return true;
}

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

//get user input on path of file to upload
//open file and send it to server
std::string Client::getUploadPath() {
	std::string path;

	cout << "Please enter the path to the file you would like to upload: ";
	std::getline(std::cin, path);
	cout << '\n';

	//look for file on thier system
	ifstream file(path.c_str());

	while (!file) {
		cout << "Error while opening the file" << endl;
		
		cout << "Reenter the path to the file: ";
        	std::getline(std::cin, path);
        	cout << '\n';

       		ifstream file(path.c_str());
	}
	
	return path;
}

long Client::getFileSize(std::string path) {

	FILE *upload_file;
	unsigned long file_size;

	upload_file =  fopen(path.c_str(), "r");
	
	if (upload_file == NULL) {
		cout << "File not found" << endl;
	} else {
		cout << "File " << upload_file << " found" << endl;
		
		fseek(upload_file, 0, SEEK_END);
		file_size = ftell(upload_file);
		cout << "File has " << file_size << " bytes." << endl;
		rewind(upload_file);
	}
	
	//send server message stating the file size
	
	
	fclose(upload_file);
	return file_size;
}


void Client::sendUploadFile(std::string path){

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

	//connect to server and start downloading file?
	
	return fileNum;
}

//close connection to server
void Client::close_connection() {
	close(sock);
}

int main(int argc, char *argv[]) {

	Client c;
	c.connection(argc, argv);

/*
	char upORdown = 'a';
	getUserData(upORdown);
	if (upORdown == 'u') {
		//read in the path to the file the user would like to upload
		c.getUploadPath();
		//send server message that user wants to upload
		c.send();
		c. sendUploadFile();
	} else {
		//display the options of files to be downloaded
		//get user input on which file they would like to download
		c.chooseDownloadFile();
		//send client message that user wants to download
		//server sends message of number of packages to be sent
		c.send();
		//recieve listing from server of downloadable files
		c.recieve();
		
	}
*/

	c.close_connection();

	return 0;	
}


