#include <iostream>
#include "chunkQueue.hpp"

using namespace std;

int main() {
	ChunkQueue queue (10);
	queue.Display();
	
	vector<int> vec1;
	vec1.push_back(3);
	vec1.push_back(9);
	queue.updateQueue(vec1);
	queue.Display();

	vector<int> vec2;
	vec2.push_back(7);
	queue.updateQueue(vec2);
	queue.Display();

	queue.remove(0);
	queue.Display();
	
	queue.remove(5);
	queue.Display();
	return 0;
}
