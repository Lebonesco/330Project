// main.cpp
// Created By: Anisha Aggarwal

#include <string>
#include <iostream>
#include <vector>
#include <unistd.h>
#include "client.hpp"
//#include "../peer/Peer.hpp"
//#include "metafile.hpp"
#include "../metafile/encoding.cpp"
using namespace std;
using namespace metafile;

int main(int argc, char * argv[]) {

        Client c;
        int size = 512;
        string message;
        string path;
	string updatePort;
        vector<string> ports;
	vector<Peer*> peers;
        bool connected = false;
        string peer_info;
        string bencoded_info;
	string garbage;
	
        connected = c.connection();
        if (connected == false) {
                return -1;
        }

	metafile::Metafile* m = new Metafile(path);
	// this instance needs to happen for upload and download
	// assume this is going to be seeder
	ports.push_back("9510");
//	Peer* seeder = new Peer(m->chunkNumber, "9510", ports, "Seeder"); 
/*
	pid_t pid = fork();
	if (pid == 0) {
		seeder->startSeeding(seeder->selfIP, seeder->selfPort);
	}
	peers.push_back(seeder);
*/
	char upORdown = 'a';
//	while (upORdown != 'q') {
		c.getUserData(upORdown);
		if (upORdown == 'u') {
			//send server message that user wants to upload
			message = "upload";
			c.sendStringData(message);

			//read in the path to the file the user would like to upload		
			path = c.getUploadPath();
/*	
		Peer* seeder = new Peer(m->chunkNumber, "9000", ports, "Seed"); 
		//encode port number
		seeder->selfPort = "9000";
		ports.push_back(seeder->selfPort);
		cout << c.sendStringData(encodeStr("9000")) << endl;
		seeder->startSeeding(seeder->selfIP, seeder->selfPort);
		peers.push_back(seeder);
*/
//		seeder->startSeeding(seeder->selfIP, seeder->selfPort);
			//read in garbage message
			//garbage = c.receive(20);

			//encode the path
			message = encodeSeeder(9510, path);
			c.sendStringData(message);

		} else if (upORdown == 'd') {
			//send client message that user wants to download
			message = "download";
			c.sendStringData(message);
/*
		if (ports[0] != "9000") {
			Peer* seeder = new Peer(m->chunkNumber, "9000", ports, "Seed"); 
			//encode port number
			seeder->selfPort = "9000";
			ports.push_back(seeder->selfPort);
			cout << c.sendStringData(encodeStr("9000")) << endl;
			seeder->startSeeding(seeder->selfIP, seeder->selfPort);
			peers.push_back(seeder);
		}
*/
//			seeder->startSeeding(seeder->selfIP, seeder->selfPort);

			//display the options of files to be downloaded
        	        //get user input on which file they would like to download
                	//c.chooseDownloadFile();
	                c.receive(size);
        	        //peer_info = decode(bencoded_info);
                	//cout << bencoded_info << endl;
/*
			//peer class starts leeching
			ports.push_back("9511");
			Peer* leecher1 = new Peer(m->chunkNumber, "9511", ports, "Leech");
			peers.push_back(leecher1);
			leecher1->startLeeching(leecher1->portList);
			c.sendStringData("9511");

			ports.push_back("9512");
			Peer* leecher2 = new Peer(m->chunkNumber, "9512", ports, "Leech");
			peers.push_back(leecher2);
			leecher2->startLeeching(leecher2->portList);
			c.sendStringData("9512");

			ports.push_back("9513");
			Peer* leecher3 = new Peer(m->chunkNumber, "9513", ports, "Leech");
			leecher3->startLeeching(leecher3->portList);
			peers.push_back(leecher3);
			c.sendStringData("9513");

			//constantly listen for update list from server to send to peer
			while(!c.filesComplete(peers)) {
				//iterate through peers vector
				//update peer list
				updatePort = c.receive(4);
				ports = decode(updatePort);
				for (int i = 0; i < peers.size(); ++i) {
                        		//cout << peers[i] << endl;
					peers[i]->updatePortList(ports);
                		}
			}
*/                	//server sends message of number of packages to be sent
                	//recieve listing from server of downloadable files
                	//cout << c.receive(size) << endl;

        	}
		upORdown = 'a';
//	}
	//user said to quit
//	message = "quit";
//	c.sendStringData(message);

        c.close_connection();
	exit(0);

	return 0;
}



