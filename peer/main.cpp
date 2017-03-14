#include <iostream>
#include "chunkQueue.hpp"

using namespace std;

int main() {
	ChunkQueue queue (10);
	queue.Display();
	
	vector<int> vec1;
	vec1.push_back(1);
	vec1.push_back(1);
	vec1.push_back(0);
	vec1.push_back(0);
	vec1.push_back(0);
	vec1.push_back(0);
	vec1.push_back(0);
	vec1.push_back(1);

	vector<int> vec2;
	vec2.push_back(7);
	vec2.push_back(1);
	vec2.push_back(1);
	vec2.push_back(0);
	vec2.push_back(0);
	vec2.push_back(0);
	vec2.push_back(0);
	vec2.push_back(0);
	vec2.push_back(1);
	queue.updateQueue(vec1, vec2);
	queue.Display();

	queue.remove(0);
	queue.Display();
	
	queue.remove(5);
	queue.Display();
	return 0;
}
