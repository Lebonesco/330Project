#include <string>
#include "metafile.hpp"
#include <iostream>

using namespace metafile;

int main(int argc, char * argv[]) {
	string filename;
	cout << "Please type in file path: " << endl;
	cin >> filename;
	Metafile *metafile = new metafile::Metafile(filename);
	(*metafile).displayMetafile();
}
