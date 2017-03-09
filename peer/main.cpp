#include <iostream>
#include "chunkQueue.hpp"

using namespace std;

int main() {
	ChunkQueue queue (10);
	
	vector<int>* items = queue.getQueue();
	cout << (*items)[0] << endl;
	cout << (*items).size() << endl;
	cout << (*items).size() << endl;

	vector<int>::iterator it = (*items).begin();
	//cout << (*items).size() << endl;

	for(it = (*items).begin(); it != (*items).end(); ++it) {
		//cout << (*it) << endl;
		//cout << (*items)[(*it)] << endl;
	}
	return 0;
}
