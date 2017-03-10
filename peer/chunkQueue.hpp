#ifndef CHUNKQUEUE_HPP_
#define CHUNKQUEUE_HPP_

#include <string>
#include <map>
#include <vector>
#include <set>

using namespace std;

class Chunk {
public:
	int key;
	int val;
		
Chunk(int key, int val) {
	key = key;
	val = val;
}
				
void updateVal(int update) {
	val = update;
}
					
void updateKey(int update) {
	key = update;
}
						
};

class ChunkQueue {
	public:
		int size;
		vector<Chunk> queue;
		map<int, int> *indexMap;

	public:
		ChunkQueue(int max);
		~ChunkQueue();
		int getSize();
		vector<Chunk> getQueue();
		int getChunk(set<int> values);
		void updateQueue(vector<int> values);
		void Display();
	private:
		int getIndex(int val);
		void BubbleDown(int idx);
		void remove(int idx);
		void swap(int idx1, int idx2);
		void increaseKey(int idx, int update);
};

#endif
