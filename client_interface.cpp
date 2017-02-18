//Client interface to be able to connect to server/other peers

#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
using namespace std;

void getUserData(int s_port, int c_port, char u_or_d) {
	
	cout << "Please enter the server port number you would like to connect to: ";
	cin >> s_port;
	cout << "Server Port:  " << s_port << endl;
	

	cout << "Please enter in your desired client port number between (___ ): "; 
	cin >> c_port;
	cout << "Client Port:  " << c_port << endl;

	//check to make sure user entered either upload or download
	while ((strcmp(u_or_d, 'd') != 0) || (strcmp(u_or_d, 'u') != 0)) {
		cout << "Would you like to upload(u) or download(d) a file? ";
        	cin >> u_or_d;
        	cout << "Upload or Download: " << u_or_d << endl;
	}
	
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
	int serverPort;
	int clientPort;
	char upORdown = 'a';

	getUserData(serverPort, clientPort, upORdown);
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


