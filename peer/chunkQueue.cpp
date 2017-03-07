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

void swap(int idx1, int idx2) {
	int val1 = queue[idx1];
	int val2 = queue[idx2];

	indexMap[val1] = idx1;
	indexMap[val2] = idx2;

	int tmp = queue[idx1];
	queue[idx1] = queue[idx2];
	queue[idx2] = tmp;
}

void increaseKey(int idx, int update) {
	queue[idx] == update

	while(idx > 0) {	
		parentIdx = parentIndex(idx);

		if(queue[idx] > queue[parentIdx]) {
			swap(idx, parentIdx);
			idx = parentIdx;
		} else {
			break;
		}
	}
	
}

void BubbleDown(int idx) {
	int length = queue.size();
	int leftChild = leftChildIndex(idx);
	int rightChild = rightChildIndex(idx);

	if(leftChild >= length) {
		return;
	}

	int maxIndex = idx;

	if(queue[idx] < queue[leftChild]) {
		maxIndex = leftChild;
	}

	if((rightChild < length) && (queue[maxIndex] < queue[rightChild])) {
		maxIndex = rightChild;
	}

	if(maxIndex != idx) {
		// need to swap
		int tmp = queue[idx];
		queue[idx] = queue[maxIndex];
		queue[maxIndex] = tmp;
		BubbleDown(maxIndex);
	}

	
}

void remove(int idx) {
	int length = queue.size();
	
	if(length == 0) {
		return;
	}
	
	queue[idx] = queue[length-1];
	queue.pop_back();
	BubbleDown(0);

	}

}

void getChunk(set pos) {
	
	<vector<int>>::iterator it = queue.begin();
	for(it = queue.begin(); it != queue.end(); ++it) {
		const bool is_in = pos.find((*it) != pos.end());
		if(is_in) {
			// delete from queue and table
			int idx = indexMap(*it);
			size--;
			remove(idx);
			return (*it);
		}
	}
}



}
