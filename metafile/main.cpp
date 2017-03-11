#include <string>
#include "metafile.hpp"
#include <iostream>

using namespace metafile;

int main(int argc, char * argv[]) {
	string filename;
	cout << "Please type in file path: " << endl;
	cin >> filename;
	Metafile *file = new metafile::Metafile(filename);
	cout << "chunk number is: " << (*file).getChunkNumber() << endl;
	cout << "BitField" << endl;
	vector<char*> bitfield = (*file).getBitfield();


}
