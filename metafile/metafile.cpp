#include <string>
#include <fstream>
#include <iostream>
#include "metafile.hpp"

using namespace std;
using namespace metafile;
//ifstream::ate - set the initial position at the end of the file
//ifteram::bianry - open in binary mode
Metafile::Metafile(string filename){
	getFileSize(filename);
	fileName = filename;

	bencode();
}

void Metafile::getFileSize(string filename) {
	ifstream mySource;
	mySource.open(filename, ios_base::binary);
	mySource.seekg(0, ios_base::end);
	int size = mySource.tellg();
	mySource.close();
	fileSize = size;
	pieceSize = size / 10;
}

void Metafile::displayMetafile() {
	cout << "Displaying Metafile Data:\n " << endl;
	cout << "\nnumber of prefered Neighbors: " << numberOfPreferedNeighbors << endl;
	cout << "\nunchoking interval: " << unchokingInterval << endl;
	cout << "\noptimistic unchoking: " << optimisticUnchoking << endl;
	cout << "\nfile size(bytes) : " << fileSize << endl;
	cout << "\npiece size(bytes) : " << pieceSize << endl;
	cout << "\nfile name: " << fileName << endl;
}

void Metafile::bencode() {
	
}
