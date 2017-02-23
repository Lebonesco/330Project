#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "metafile.hpp"
#include <cstring>
//#include "bencode.h"
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
	ifstream::pos_type nSize; // keep tracks of file length
	ifstream fSource(fileName, ios_base::ate | ios::binary | ios::in); // opens file
	char *sMemBlock;
	nSize = fSource.tellg(); // get size of file
	string Extension = strstr(fileName.c_str(), ".");
	int nGetPointer = 0;
	istringstream nIntegerer(to_string(pieceSize));
	int nChunkSize;
	nIntegerer >> nChunkSize; // assign chunk size
	int nLastChunkSize = nChunkSize;
	int nPartNumber = 1;
	string sDestinationFile;
	string str = "none";
	char *none = &str[0u];
	vector<char*> tmp((nSize / nChunkSize) + 1, none);
	bitfield = &tmp;

	if(fSource.is_open()) { // check if file is open
		fSource.seekg(0, ios::beg); // get beginning of file
		while(fSource.tellg() < nSize){ // while there is still data to export

		fSource.seekg(nGetPointer, ios::beg); // increment placement in stream
		if(nGetPointer + nChunkSize > nSize) { // if handling last chunk
			while(nGetPointer + nLastChunkSize > nSize) {
				nLastChunkSize--;
			}
			sMemBlock = new char[nLastChunkSize];
			fSource.read(sMemBlock, nLastChunkSize);
			//(*bitfield)[nPartNumber] = sMemBlock;
		} else {

			sMemBlock = new char[nChunkSize]; // assign memory
			fSource.read(sMemBlock, nChunkSize); // read file data into memory
			(*bitfield)[nPartNumber-1] = sMemBlock;
		}
		
		sDestinationFile = fileName;
		sDestinationFile.append(".part");
		sStringer.str("");
		sStringer << nPartNumber;
		sDestinationFile.append(sStringer.str());

		cout << "Destination file: " << sDestinationFile << endl;
		cout << "Chunk Size: " << nLastChunkSize << endl;
		ofstream fDestination(sDestinationFile.c_str()); // create new file
		fDestination.write(sMemBlock, nLastChunkSize); // write memory to file

		nGetPointer += nChunkSize; // increment file stream pointer
		nPartNumber += 1; 
		}

		cout << "map data: " << endl;
		vector<char*>::iterator it = (*bitfield).begin();

		for(it = (*bitfield).begin(); it != (*bitfield).end(); ++it) {
			cout << (*it) << endl;			
		}
		
	combineChunks(nPartNumber-1);
	}
}

void Metafile::combineChunks(int nParts) {
	string sChunkFile;
	ostringstream sStringer;
	int nPartNumber = 1;
	char * sMemBlock;
	ifstream::pos_type nSize;

	ofstream fRetour("recupe.avi"); // create destination file
	for(nPartNumber = 1; nPartNumber <= nParts; nPartNumber++) {
		cout << "file: " << nPartNumber << endl;
		sChunkFile = "file.txt";
		sChunkFile.append(".part");
		sStringer.str("");
		sStringer << nPartNumber;
		sChunkFile.append(sStringer.str());
		ifstream fChunk(sChunkFile.c_str(), ios::in|ios::binary|ios::ate); // open target file
		nSize = fChunk.tellg(); // get size of file
		sMemBlock = new char[nSize]; // allocate memory 
		fChunk.seekg(0, ios::beg); // set stream to beginning
		fChunk.read(sMemBlock, nSize); // read file to memory block
		fChunk.close();	// close file stream
		fRetour.write(sMemBlock, nSize); // write blockt to destination file
	}
	fRetour.close();
}
