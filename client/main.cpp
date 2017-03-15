// main.cpp
// Created By: Anisha Aggarwal

#include <string>
#include <iostream>
#include "client_interface.cpp"
#include "Peer.cpp"
#include "metafile.cpp"

using namespace std;
//using namespace metafile;

int main(int argc, char * argv[]) {

        Client c;
        int size = 512;
        std::string message;
        std::string path;
        std::vector<std::string> ports;
	//std::vector<peer::Peer* p> peers;
        bool connected = false;
        std::string peer_info;
        std::string bencoded_info;
	std::string garbage;
	
        connected = c.connection(argc, argv);
        if (connected == false) {
                return -1;
        }

	Metafile* m = new Metafile(path);
	// this instance needs to happen for upload and download
	// assume this is going to be seeder
	Peer* seeder = new Peer(m->chunkNumber, "9000", ports, "seeder"); 
	seeder.startSeeding();

	char upORdown = 'a';
	getUserData(upORdown);
	if (upORdown == 'u') {
		//send server message that user wants to upload
		message = "upload";
		c.sendStringData(message);

		//read in the path to the file the user would like to upload		
		path = c.getUploadPath();

		//encode port number
		seeder->selfPort = "9000";
		ports.push_back(seeder->selfPort);
		//cout << c.sendStringData(encode(seeder->selfPort)) << endl;

		//read in garbage message
		garbage = c.receive(20);

		//encode the path
		//message = encode(path);
		message = encode(9000, path);
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
		Peer* leecher1(m.chunkNumber, 9001, ports, "leecher");
		peers.pushback(leecher1);
		leecher1.startLeeching();
		c.sendStringData("9001");

		Peer* leecher2(m.chunkNumber, 9002, ports, "leecher");
		peers.pushback(leecher2);
		leecher2.startLeeching();
		c.sendStringData("9002");

		Peer* leecher3(m.chunkNumber, 9003, ports, "leecher");
		peers.pushback(leecher3);
		leecher3.startLeeching();
		c.sendStringData("9003");

		for (std::vector<Peer* p>::iterator it = peers.begin(); it != peers.end(); ++it) {
			cout << it << endl;
		}

		//constantly listen for update list from server to send to peer
		while(!filesComplete) {
			//iterate through peers vector
			//update peer list
			std::string updatePort = c.receive(4);
			for (std::vector<Peer* p>::iterator it = peers.begin(); it != peers.end(); ++it) {
                        	cout << it << endl;
				it.updatePortList(updatePort);
                	}
		}
			
                //server sends message of number of packages to be sent
                //recieve listing from server of downloadable files
                //cout << c.receive(size) << endl;

        }

        c.close_connection();

	return 0;
}



