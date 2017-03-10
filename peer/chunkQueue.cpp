#include <vector>
#include <map>
#include <set>
#include "chunkQueue.hpp"
#include <iostream>
using namespace std;

	
ChunkQueue::ChunkQueue(int max) {
	size = max;
	vector<Chunk> tmp(size, Chunk(1,1));
	queue = tmp;
	map<int, int> tmpMap;
	indexMap = tmpMap;
	int i;
	for(i =  0; i < max; ++i) {
		queue[i].key = i;
		queue[i].val = 1;
		indexMap.insert(map<int, int>::value_type(i, i));
	}

}

ChunkQueue::~ChunkQueue() {}

vector<Chunk> ChunkQueue::getQueue() {
	return queue;
}
	
int ChunkQueue::getIndex(int val) {
	return indexMap[val];
} 

int ChunkQueue::getSize() {
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

void ChunkQueue::swap(int idx1, int idx2) {
	Chunk val1 = queue[idx1];
	Chunk val2 = queue[idx2];

	indexMap[val1.key] = idx2;
	indexMap[val2.key] = idx1;
	//cout << "key: " << val1.key << " is now at index " << idx2 << endl;
 	//cout << "key: " << val2.key << " is now at index " << idx1 << endl;

	Chunk tmp = queue[idx1];
	queue[idx1] = queue[idx2];
	queue[idx2] = tmp;
}

void ChunkQueue::increaseKey(int idx, int update) {
	queue[idx].val += update;
	while(idx > 0) {	
		int parentIdx = parentIndex(idx);
		if(queue[idx].val > queue[parentIdx].val) {
			swap(idx, parentIdx);
			idx = parentIdx;
		} else {
			break;
		}
	}
	
}

void ChunkQueue::updateQueue(vector<int> chunks) {
	cout << "Update Rarity" << endl;
	vector<int>::iterator it = chunks.begin();
	for(it = chunks.begin(); it != chunks.end(); ++it) {
		int idx = indexMap[(*it)];
		int update = 1;
		increaseKey(idx, update);
	}
}


void ChunkQueue::BubbleDown(int idx) {
	int length = queue.size();
	int leftChild = leftChildIndex(idx);
	int rightChild = rightChildIndex(idx);

	if(leftChild >= length) {
		return;
	}

	int maxIndex = idx;

	if(queue[idx].val < queue[leftChild].val) {
		maxIndex = leftChild;
	}

	if((rightChild < length) && (queue[maxIndex].val < queue[rightChild].val)) {
		maxIndex = rightChild;
	}

	if(maxIndex != idx) {
		swap(idx, maxIndex);
		BubbleDown(maxIndex);
	}

	
}

void ChunkQueue::remove(int idx) {
	int length = queue.size();
	
	if(length == 0) {
		return;
	}
	
	queue[idx] = queue[length-1];
	queue.pop_back();
	BubbleDown(0);

}


int ChunkQueue::getChunk(set<int> pos) {
	
	vector<Chunk>::iterator it = queue.begin();
	for(it = queue.begin(); it != queue.end(); ++it) {
		const bool is_in = pos.find((*it).key) != pos.end();
		if(is_in) {
			// delete from queue and table
			int idx = indexMap[(*it).key];
			size--;
			remove(idx);
			return (*it).key;
		}
	}
}

void ChunkQueue::Display() {
	cout << "Chunk ordering: " << endl;
	vector<Chunk>::iterator it = queue.begin();
	for(it = queue.begin(); it != queue.end(); ++it) {
		cout << (*it).key << " " << flush;
	}
	cout << "" << endl;


}
