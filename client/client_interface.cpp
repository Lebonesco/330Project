//Client interface to be able to connect to server/other peers

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <errno>
#include <unistd>
#include <netdb>
#include <sys/types>
#include <netinet/in>
#include <sys/socket>
#include <arpa/inet>
using namespace std;

#define PORT 3490
#define MAXDATASIZE 100

void getUserData(char u_or_d) {

	//check to make sure user entered either upload or download
	while ((strcmp(u_or_d, 'd') != 0) || (strcmp(u_or_d, 'u') != 0)) {
		cout << "Would you like to upload(u) or download(d) a file? ";
        	cin >> u_or_d;
        	cout << "Upload or Download: " << u_or_d << endl;
	}
	
}

//connect to the server using socket connections
Client::get_in_addr(struct sockaddr *sr) {
	if (sa->sa_family === AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

//check if connection was successful
Client::connection_made() {
	if (argc != 2) {
        	fprintf(stderr,"usage: client hostname\n");
        	exit(1);
    	}

    	memset(&hints, 0, sizeof hints);
    	hints.ai_family = AF_UNSPEC;
    	hints.ai_socktype = SOCK_STREAM;

    	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        	return 1;
    	}

    	// loop through all the results and connect to the first we can
    	for(p = servinfo; p != NULL; p = p->ai_next) {
    	    if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            	perror("client: socket");
            	continue;
        	}
	
            if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            	close(sockfd);
            	perror("client: connect");
            	continue;
            }

            break;
    	}

    	if (p == NULL) {
        	fprintf(stderr, "client: failed to connect\n");
        	return 2;
    	}

    	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr), s, sizeof s);
    	printf("client: connecting to %s\n", s);

    	freeaddrinfo(servinfo); // all done with this structure
	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
        	perror("recv");
        	exit(1);
    	}

    	buf[numbytes] = '\0';

    	printf("client: received '%s'\n",buf);

}

//close connection to server
Client::close_connnection() {
	close(sockfd);
}

//get user input on path of file to upload
void getPath() {
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
	
	//upload file onto server?
	
}
//get user input on what file they would like to download
void getDownloadFile() {
	int fileNum;

	cout << "Enter number of file would you like to download: ";
	cin >> fileNum;
	cout << "File number entered: " << fileNum << endl;

	//connect to server and start downloading file?

}

int main () {
	char upORdown = 'a';

	getUserData( upORdown);
	if (upORdown == 'u') {
		//read in the path to the file the user would like to upload
		getPath();
	} else {
		//display the options of files to be downloaded
		//get user input on which file they would like to download
		getDownloadFile();
	}
	

	return 0;	
}


