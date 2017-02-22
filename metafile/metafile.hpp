#ifndef METAFILE_HPP_
#define METAFILE_HPP_

using namespace std;

namespace metafile {
class Metafile {
public:
	Metafile(string file);
	~Metafile();
	void getMetafile();
	void displayMetafile();
	void getFileSize(string file);
	void bencode();
	void generateChunks();
	void combineChunks(int nParts);
public:
	int numberOfPreferedNeighbors = 2;
	int unchokingInterval = 5;
	int optimisticUnchoking = 15;
	int fileSize;
	int pieceSize;
	string fileName;
	string announce;
};
};

#endif
