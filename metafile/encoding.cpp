#include <vector>
#include <string>
#include <iostream>
#include <typeinfo>

using namespace std;

string integerEncode(int x) {
	string r;
	r.append("i");
	r.append(x);
	r.append("e");
	return r;
}

string stringEncode(string x) {
	string r;
	length = x.length();
	r.append(length);
	r.append(x);
	return r;
}

template <typename T>
template <typename K>
string mapEncode(map<T, K> x) {
	string r;
	string kType, vType, k, v;
	r.append("d");
	map<T, K>::iterator it;
	
	for(it = x.begin(); it != x.end(); ++it) {
		r.append(encode((*it)));
		r.append(encode(x[(*it)]));
	}
	r.append("e");
	return r;
}

template <typename T>
string encode(T x) {
	string r;
	string type = typeid(x).name();
	if(type == i) {
		r = integerEncode(x);
	} else if(type == s) {
		r = stringEncode(x);

	} else if(type == d) {
		r = mapEncode(x);

	} else {
		cout << "failure" << endl;
	}
	
	return r;
}

int main(){

	return 0;
}
