#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "metafile.hpp"
#include <cstring>

using namespace std;
using namespace metafile;
//ifstream::ate - set the initial position at the end of the file
//ifteram::bianry - open in binary mode
Metafile::Metafile(string filename){
	getFileSize(filename);
	fileName = filename;

	generateChunks();
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

void Metafile::generateChunks() {
	std::ostringstream sStringer;
	ifstream::pos_type nSize;
	ifstream fSource(fileName, ios_base::ate | ios::binary | ios::in);
	char *sMemBlock;
	nSize = fSource.tellg();
	string Extension = strstr(fileName.c_str(), ".");
	int nGetPointer = 0;
	istringstream nIntegerer(to_string(pieceSize));
	int nChunkSize;
	nIntegerer >> nChunkSize;
	int nLastChunkSize = nChunkSize;
	int nPartNumber = 1;
	string sDestinationFile;
	
	if(fSource.is_open()) {
		fSource.seekg(0, ios::beg);
		while(fSource.tellg() < nSize){

		fSource.seekg(nGetPointer, ios::beg);
		if(nGetPointer + nChunkSize > pieceSize) {
			while(nGetPointer + nLastChunkSize > nSize) {
				nLastChunkSize--;
			}
			sMemBlock = new char[nLastChunkSize];
			fSource.read(sMemBlock, nLastChunkSize);	
		} else {

			sMemBlock = new char[nChunkSize];
			fSource.read(sMemBlock, nChunkSize);
		}

		sDestinationFile = fileName;
		sDestinationFile.append(".part");
		sStringer.str("");
		sStringer << nPartNumber;
		sDestinationFile.append(sStringer.str());

		cout << "Destination file: " << sDestinationFile << endl;
		cout << "Chunk Size: " << nLastChunkSize << endl;
		ofstream fDestination(sDestinationFile.c_str());
		fDestination.write(sMemBlock, nLastChunkSize);

		nGetPointer += nChunkSize;
		nPartNumber += 1;
		}
	}
}
