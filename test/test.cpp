#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <unistd.h>
#include <vector>
#include "metafile.hpp"

using namespace std;
using namespace metafile;

void peerConnection(int client, string f) {
	int i;
	usleep(3000000);
	cout << "client " << client << " beginning download of " << f << "..." << endl;
	usleep(1000000);
	cout << "grabbing list of peers from server..." << endl;
	for(i = 0; i != 5; ++i) {
		int num = rand() % 10 + 1;
		if(i != num) {
			usleep(5000000);
			cout << "connecting to peer " << num << endl;
			usleep(5000000);
			cout << "checking peer " << num << " bitfield" << endl;
			cout << "updating rariest chunk" << endl;
			usleep(5000000);
			cout << client << " is grabbing chunk from " << num << endl;
		}
	}

	cout << "download finished..." << endl;
	usleep(1000000);
	cout << "generating chunks..." << endl;
}

int main() {
	string r = "";
	while(r != "u" && r != "d") {
		cout << "Would you like to upload(u) or download(d) a file? ";
		getline(cin, r);
		if( r != "u" && r != "d") {
			cout << "Invalid command. Please try again. " << endl;
		}
	}

	if(r == "u") {
		cout << "Please choose a file to upload." << endl;
		string file;
		getline(cin, file);

		cout << "Uploading " << file << " to the server" << endl;
		usleep(3000000);
		cout << "connecting to server..." << endl;
		usleep(3000000);
		cout << "server connected.." << endl;
		usleep(3000000);
		cout << "creating peer list directory for file " << file << endl;
		usleep(3000000);
		cout << "directory created" << endl;
		usleep(3000000);
		cout << "peers can now download file: " << file << endl;
		cout << "launching cluster of peers..." << endl;
		
		int i;
		vector<unique_ptr<thread> > threads;
		for(i = 0; i != 10; ++i) {
			threads.push_back(unique_ptr<thread>
				(new thread(peerConnection, i, file))); 
		}

		for(i = 0; i < 10; ++i) {
			threads[i]->join(); // wait for thread to finish
		}
		Metafile *f = new Metafile("file.txt");
		(*f).getChunkNumber();
		while(true) {
			usleep(3000000);
			cout << "server looking for connection..." << endl;
		}
	}	

	return 0;
}
