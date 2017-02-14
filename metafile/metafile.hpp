#ifndef METAFILE_H
#define METAFILE_H

using namespace std;

namespace metafile {
class Metafile {
public:
	Metafile(string file);
	~Metafile();
	void getMetafile();
	void displayMetafile();
protected:
	int numberOfPreferedNeighbors = 2;
	int Unchoking interval = 5;
	int optimisticUnchoking = 15;
	int fileSize;
	int pieceSize;
	string fileName;
	string announce;
};
}







#endif
