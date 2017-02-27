#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

string encode(int x) {
	string r;
	r.append("i");
	r.append(to_string(x));
	r.append("e");
	return r;
}

string encode(string x) {
	string r;
	int length = x.length();
	r.append(length);
	r.append(x);
	return r;
}

template <typename K, typename T>
string encode(map<T, K> x) {
	string r;
	r.append("d");
	map<T, K>::iterator it = x.begin();
	
	for(it = x.begin(); it != x.end(); ++it) {
		r.append(encode((*it)));
		r.append(encode(x[(*it)]));
	}
	r.append("e");
	return r;
}

template <typename T>
string encode(vector<T> x) {
	string r;
	r.append("l");
	vector<T>::iterator it = x.begin();
	
	for(it = x.begin(); it != x.end(); ++it) {
		r.append(encode((*it)));
	}
	r.append("e");
	return r;
}

int main(){
	string r;	
	int tmp = 5;
	r = encode(tmp);
	cout << r << endl;

	return 0;
}
