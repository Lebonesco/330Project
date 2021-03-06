#ifndef METAFILE_HPP_
#define METAFILE_HPP_

#include <vector>
#include <string>

using namespace std;

namespace metafile {
class Metafile {
public:
	Metafile(string file);
	~Metafile();
	void displayMetafile();
	int getChunkNumber(); // this needs to get called to return chunk number
	void generateChunks();
	void combineChunks(int nParts);
	vector<char*> getBitfield(); // returns bitfield array filled with data
public:
	static const int numberOfPreferedNeighbors= 2;
	static const int unchokingInterval = 5;
	static const int optimisticUnchoking = 15;
	int fileSize;
	int pieceSize;
	int chunkNumber;
	vector<char*> bitfield;
	string fileName;
	string announce;
};
};

#endif
