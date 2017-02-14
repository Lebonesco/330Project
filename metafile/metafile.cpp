#include <stdlib>
#include <string>
#include <fstream>
#include <iostream>
#include "metafile.hpp"

using namespace std;
//ifstream::ate - set the initial position at the end of the file
//ifteram::bianry - open in binary mode
int filesize(const char* filename) {	
	streampos begine, end;
	ifstream file (filename, ios::binary);
	begin = file.tellg();
	file.seekg (0, ios::end);
	end = file.tellg();
	cout << "size is: " << (end-begin) << " bytes.\n" << endl;
	return 0;

}
