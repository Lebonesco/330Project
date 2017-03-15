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
		cout << c.sendStringData(encode(seeder->selfPort)) << endl;

		//read in garbage message
		c.receive(garbage);

		//encode the path
		message = encode(path);
		c.sendStringData(message);

	} else {
		//send client message that user wants to download
		message = "download";
		c.sendStringData(message);

		//display the options of files to be downloaded
/*
        char upORdown = 'a';
        getUserData(upORdown);
        if (upORdown == 'u') {
                //send server message that user wants to upload
                message = "upload";
                c.sendStringData(message);


                //read in the path to the file the user would like to upload
                path = c.getUploadPath();

                //encode port number
                port = encode(4);
                cout << c.sendStringData(port) << endl;
                //port = encode(p.port);
                //c.sendStringData(port);

                //encode the path
                message = encode(path);
                c.sendStringData(message);

        } else {
                //send client message that user wants to download
                message = "download";
                c.sendStringData(message);

                //display the options of files to be downloaded
*/
                //get user input on which file they would like to download
                //c.chooseDownloadFile();
                bencoded_info = c.receive(size);
                //peer_info = decode(bencoded_info);
                cout << bencoded_info << endl;

//        	Peer *p = new Peer(const int numChunks, p.port, vector<string>& recvPortList, string type);


                //connect to server and start downloading file?
                //peer class?

                //server sends message of number of packages to be sent
                //recieve listing from server of downloadable files
                //cout << c.receive(size) << endl;

        }

        c.close_connection();

	return 0;
}



