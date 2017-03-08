#ifndef QUEUE_HPP_
#define QUEUE_HPP_

#include <string>
#include <map>
#include <vector>

using namespace std;

class ChunkQueue {
	public:
		int size;
		vector<int> queue;
		map<int, int> indexMap;

	public:
		ChunkQueue(int max);
		~ChunkQueue();
		int getSize();
		vector<int> getQueue();
		int getChunk(set<int> values);
		void updateQueue(vector<int> values);
};

#endif
