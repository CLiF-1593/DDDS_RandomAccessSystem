#include <iostream>
#include <vector>
#include <FileIO.h>
#include <algorithm>
using namespace std;

class ReturnType {
public:
	int distance = -1;
	string org;
	string dat;

	bool operator <(ReturnType& other) {
		return this->distance < other.distance;
	}
};

vector<ReturnType> cache;

ReturnType EditDistance(string& org, string& dat, int index1, int index2) {
	ReturnType ret;
	if (cache[(dat.size() + 1) * index1 + index2].distance != -1) {
		return cache[(dat.size() + 1) * index1 + index2];
	}
	if (index1 + index2 == 0) {
		ret.distance = 0;
		cache[(dat.size() + 1) * index1 + index2] = ret;
		return ret;
	}
	if (index1 == 0) {
		ret.distance = index2;
		for (int i = 0; i < index2; i++) ret.org.push_back(0);
		ret.dat.insert(ret.dat.end(), dat.begin(), dat.begin() + index2);
		cache[(dat.size() + 1) * index1 + index2] = ret;
		return ret;
	}
	if (index2 == 0) {
		ret.distance = index1;
		ret.org.insert(ret.org.end(), org.begin(), org.begin() + index1);
		for (int i = 0; i < index1; i++) ret.dat.push_back(0);
		cache[(dat.size() + 1) * index1 + index2] = ret;
		return ret;
	}
	vector<ReturnType> container;
	container.push_back(EditDistance(org, dat, index1 - 1, index2 - 1));
	container.push_back(EditDistance(org, dat, index1 - 1, index2));
	container[0].distance += (org[index1 - 1] != dat[index2 - 1]);
	container[0].org.push_back(org[index1 - 1]);
	container[0].dat.push_back(dat[index2 - 1]);
	container[1].distance++;
	container[1].org.push_back(org[index1 - 1]);
	container[1].dat.push_back(0);
	sort(container.begin(), container.end());
	cache[(dat.size() + 1) * index1 + index2] = container.front();
	return container.front();
}

int GetDelta(string& org, string& dat) {
	cache.clear();
	cache.resize((org.size() + 1) * (dat.size() + 1));
	ReturnType ret = EditDistance(org, dat, org.size(), dat.size());
	org = ret.org;
	dat = ret.dat;
	return ret.distance;
}

int main() {
	cout << "[ DNA BER Calculator ]" << endl << endl;
	cout << "> Bit Error Rate Calculating System" << endl;
	cout << "  ¤¤Random Access of DNA Digital Data Storage System Using Restriction Modification System" << endl << endl;
	cout << "> 2023 KSA R&E" << endl << endl << endl;
	system("Pause");

	srand((unsigned)time(NULL));

	cout << "> Select the Original File" << endl;
	vector<string> org_paths = FileIO::GetFileNames();
	if(org_paths.empty()) return 0;
	string org_path = org_paths[0];

	string org_dat = FileIO::ReadFileData(org_path);

	cout << "> Select the Files" << endl;
	vector<string> path = FileIO::GetFileNames();
	if (path.empty()) return 0;

	for (int i = 0; i < path.size(); i++) {
		string dat = FileIO::ReadFileData(path[i]);
		string org = org_dat;
		GetDelta(org, dat);
		// Bit Error Rate
		double ber = 0;
		for (int j = 0; j < org.size(); j++) {
			if (dat[j]) {
				int n = org[j] ^ dat[j];
				for (int k = 0; k < 8; k++) {
					ber += (n & 1);
					n >>= 1;
				}
			}
			else if (org[j]) {
				ber += 8;
			}
		}
		ber /= org.size() * 8;
		ber *= 100;
		string p;
		while (path[i].back() != '\\') {
			p.push_back(path[i].back());
			path[i].pop_back();
		}
		reverse(p.begin(), p.end());
		cout << "> " << p << " : " << ber << "%" << endl;
	}
}