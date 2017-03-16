#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <typeinfo>
#include <map>

using namespace std;

string encodeInt(int x) {
	string r;
	r.append("i");
	r.append(to_string(x));
	r.append("e");
	return r;
}

string encodeStr(string x) {
	string r;
	int length = x.length();
	r.append(to_string(length));
	r.append(":");
	r.append(x);
	return r;
}

string encodeSeeder(int x, std::string path) {
	string r;
	r.append(to_string(x));
	r.append(":");
	r.append(path);
	return r;
}

template <typename T, typename K>
string encodeMap(map<T, K> x) {
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

vector<string> decode(string data) {
	int i = 3, j = 2;
	string item;
	vector<string> result;
	while(i < data.size()) {
		item = data.substr(i, 4);
		result.push_back(item);
		i += 6;
	}
	return result;
}
/*
int main(){
	string r;	
	int tmp = 5;
	string test = "test";
	vector<string> vTest = {"spam", "eggs"};
	r = encode(vTest);
	cout << r << endl;
	r = encode(test);
	cout << r << endl;

	vector<string> peerList = {"1234","4312","1234","1341","5341"};
	vector<string> vec;
	string t;
	t = encode(peerList);
	cout << t << endl;
	vec = decode(t);
		
	vector<string>::iterator it = vec.begin();
	cout << "[" << flush;
	for(it = vec.begin(); it != vec.end(); ++it) {
		cout << (*it) << flush;
		cout << "," << flush;
	}
	cout << "]" << endl;

	return 0;
}
*/
