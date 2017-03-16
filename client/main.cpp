// main.cpp
// Created By: Anisha Aggarwal

#include <string>
#include <iostream>
#include <vector>
#include "client.hpp"
//#include "../peer/testVersion/Peer.hpp"
//#include "metafile.hpp"
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
	
        connected = c.connection(argc, argv);
        if (connected == false) {
                return -1;
        }

	metafile::Metafile* m = new Metafile(path);
	// this instance needs to happen for upload and download
	// assume this is going to be seeder
	ports.push_back("9000");
	Peer* seeder = new Peer(m->chunkNumber, "9000", ports, "Seed"); 
	seeder->startSeeding(seeder->selfIP, seeder->selfPort);
	peers.push_back(seeder);

	char upORdown = 'a';
	c.getUserData(upORdown);
	if (upORdown == 'u') {
		//send server message that user wants to upload
		message = "upload";
		c.sendStringData(message);

		//read in the path to the file the user would like to upload		
		path = c.getUploadPath();

		//encode port number
		seeder->selfPort = "9000";
		ports.push_back(seeder->selfPort);
		cout << c.sendStringData(encode(seeder->selfPort)) << endl;

		//read in garbage message
		garbage = c.receive(20);

		//encode the path
		message = c.encode(9000, path);
		c.sendStringData(message);

	} else {
		//send client message that user wants to download
		message = "download";
		c.sendStringData(message);

		//display the options of files to be downloaded
                //get user input on which file they would like to download
                //c.chooseDownloadFile();
                bencoded_info = c.receive(size);
                //peer_info = decode(bencoded_info);
                cout << bencoded_info << endl;

                //peer class?
		//peer class starts leeching
		ports.push_back("9001");
		Peer* leecher1 = new Peer(m->chunkNumber, "9001", ports, "Leech");
		peers.push_back(leecher1);
		leecher1->startLeeching(leecher1->portList);
		c.sendStringData("9001");

		ports.push_back("9002");
		Peer* leecher2 = new Peer(m->chunkNumber, "9002", ports, "Leech");
		peers.push_back(leecher2);
		leecher2->startLeeching(leecher2->portList);
		c.sendStringData("9002");

		ports.push_back("9003");
		Peer* leecher3 = new Peer(m->chunkNumber, "9003", ports, "Leech");
		leecher3->startLeeching(leecher3->portList);
		peers.push_back(leecher3);
		c.sendStringData("9003");

		//constantly listen for update list from server to send to peer
		while(!c.filesComplete(peers)) {
			//iterate through peers vector
			//update peer list
			updatePort = c.receive(4);
			for (int i = 0; i < peers.size(); ++i) {
                        	//cout << peers[i] << endl;
				peers[i]->updatePortList(updatePort);
                	}
		}
			
                //server sends message of number of packages to be sent
                //recieve listing from server of downloadable files
                //cout << c.receive(size) << endl;

        }

        c.close_connection();

	return 0;
}



