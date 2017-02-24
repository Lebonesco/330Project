#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;



string encode(int x) {
		
}

string encode(string x) {

}

string encode(map<string, int> x) {
	string r;
	int length;
	r.append("d");
	map<string, int>::iterator it;
	
	for(it = x.begin(); it != x.end(); ++it) {
		length = (*it).length();
		r.append(length);
		r.append(":");
		r.append((*it));
	}
	r.append("e");
	return r;
}

int main(){
	
	
	


	return 0;
}
