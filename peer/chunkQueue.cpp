#include <vector>
#include <map>
#include <set>
#include "chunkQueue.hpp"
#include <iostream>
using namespace std;

	
ChunkQueue::ChunkQueue(int max) {
	size = max;
	vector<int> tmp(size, 1);
	queue = &tmp;
	map<int, int> tmpMap;
	indexMap = &tmpMap;
	int i;
	for(i =  0; i < max; ++i) {
		cout << i << endl;
		(*indexMap).insert(map<int, int>::value_type(i, i));
	}

	vector<int>::iterator it = (*queue).begin();
	for(it = (*queue).begin(); it != (*queue).end(); ++it) {
		cout << (*it) << endl;
	}
}

ChunkQueue::~ChunkQueue() {}

vector<int>* ChunkQueue::getQueue() {
	cout << (*queue).size() << endl;
	cout << (*queue).size() << endl;
	vector<int>::iterator it = (*queue).begin();
	//for(it = (*queue).begin(); it != (*queue).end(); ++it) {
	//	cout << (*it) << endl;
	//}
	return queue;
}
	
int ChunkQueue::getIndex(int val) {
	return (*indexMap)[val];
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
	int val1 = (*queue)[idx1];
	int val2 = (*queue)[idx2];

	(*indexMap)[val1] = idx1;
	(*indexMap)[val2] = idx2;

	int tmp = (*queue)[idx1];
	(*queue)[idx1] = (*queue)[idx2];
	(*queue)[idx2] = tmp;
}

void ChunkQueue::increaseKey(int idx, int update) {
	(*queue)[idx] == update;

	while(idx > 0) {	
		int parentIdx = parentIndex(idx);

		if((*queue)[idx] > (*queue)[parentIdx]) {
			swap(idx, parentIdx);
			idx = parentIdx;
		} else {
			break;
		}
	}
	
}

void ChunkQueue::BubbleDown(int idx) {
	int length = (*queue).size();
	int leftChild = leftChildIndex(idx);
	int rightChild = rightChildIndex(idx);

	if(leftChild >= length) {
		return;
	}

	int maxIndex = idx;

	if((*queue)[idx] < (*queue)[leftChild]) {
		maxIndex = leftChild;
	}

	if((rightChild < length) && ((*queue)[maxIndex] < (*queue)[rightChild])) {
		maxIndex = rightChild;
	}

	if(maxIndex != idx) {
		swap(idx, maxIndex);
		BubbleDown(maxIndex);
	}

	
}

void ChunkQueue::remove(int idx) {
	int length = (*queue).size();
	
	if(length == 0) {
		return;
	}
	
	(*queue)[idx] = (*queue)[length-1];
	(*queue).pop_back();
	BubbleDown(0);

}


int ChunkQueue::getChunk(set<int> pos) {
	
	vector<int>::iterator it = (*queue).begin();
	for(it = (*queue).begin(); it != (*queue).end(); ++it) {
		const bool is_in = pos.find((*it)) != pos.end();
		if(is_in) {
			// delete from queue and table
			int idx = (*indexMap)[(*it)];
			size--;
			remove(idx);
			return (*it);
		}
	}
}


