#include <vector>
#include <map>
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

updateVal(int update) {
	val = update;
}

updateKey(int update) {
	key = update;
}

}


class ChunkQueue {
public:
	int size;
	vector queue;
	set values;
	map indexMap;
	
ChunkQueue(int max) {
	size = max;
	queue.resize(max);
}

vector getQueue() {
	return queue;
}
	
int getIndex(int val) {
	return indexMap[val];
} 

int getSize() {
	return size;
}

int parentIndex(int idx) {
	idx++;
	idx = idx / 2;
	idx--;
	return idx;
}

int leftChildIndex(int idx) {
	idx++;
	idx = idx * 2;
	idx--;
	return idx;		
}

int rightChildIndex(int idx) {
	idx++;
	idx = idx*2;
	return idx;
}

void increaseKey(int idx, int update) {
	queue[idx] == update
	if(idx == 0) { break }

	parent = parentIndex(idx);
	while(p

	
}












}
