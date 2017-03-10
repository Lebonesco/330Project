#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <typeinfo>
#include <map>

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
	r.append(to_string(length));
	r.append(":");
	r.append(x);
	return r;
}

template <typename T, typename K>
string encode(map<T, K> x) {
	string r;
	r.append("d");
	typename map<T, K>::iterator it = x.begin();
	
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
	typename vector<T>::iterator it = x.begin();
	
	for(it = x.begin(); it != x.end(); ++it) {
		r.append(encode((*it)));
	}
	r.append("e");
	return r;
}

int main(){
	string r;	
	int tmp = 5;
	string test = "test";
	vector<string> vTest = {"spam", "eggs"};
	r = encode(vTest);
	cout << r << endl;
	r = encode(test);
	cout << r << endl;

	return 0;
}
