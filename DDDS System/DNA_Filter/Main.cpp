#include <iostream>
#include <vector>
#include <FileIO.h>
#include <array>
using namespace std;

/*
* [Restriction Enzyme Sequences]
* PstI: CTGCAG
* BamHI: GGATCC
*/

int main() {
	cout << "[ DNA Filter ]" << endl << endl;
	cout << "> DNA Filtering System" << endl;
	cout << "  ¤¤Random Access of DNA Digital Data Storage System Using Restriction Modification System" << endl << endl;
	cout << "> 2023 KSA R&E" << endl << endl << endl;
	system("Pause");

	srand((unsigned)time(NULL));

	cout << "> Select the Files" << endl;
	vector<string> path = FileIO::GetFileNames();
	if (path.empty()) return 0;

	char nucleotide[4] = { 'A', 'G', 'C', 'T' };

	vector<pair<string, int>> filters = { make_pair("CTGCAG", 0), make_pair("GGATCC",0) };

	for (int i = 0; i < path.size(); i++) {
		int detected = 0;
		string dat = FileIO::ReadFileData(path[i]);
		string seq = "";
		string result = "";
		dat += '\n';
		for (int j = 0; j < dat.size(); j++) {
			if (dat[j] == 'A' || dat[j] == 'G' || dat[j] == 'C' || dat[j] == 'T') seq += dat[j];
			else if (dat[j] == '\n') {
				if (seq.empty()) continue;
				for (int k = 0; k < filters.size(); k++) {
					for (int l = 0; l < seq.size() - filters[k].first.size(); l++) {
						bool flag = true;
						for (int m = 0; m < filters[k].first.size(); m++) {
							if (seq[l + m] != filters[k].first[m]) {
								flag = false;
								break;
							}
						}
						if (flag) {
							char& c = seq[l + filters[k].second % filters[k].first.size()];
							bool sec = (int)(filters[k].second / filters[k].first.size()) % 2;
							if (c == 'A') { //T
								if(sec) c = 'G';
								else c = 'C';
							}
							else if (c == 'G') { //C
								if (sec) c = 'A';
								else c = 'T';
							}
							else if (c == 'C') { //G
								if (sec) c = 'T';
								else c = 'A';
							}
							else if (c == 'T') { //A
								if (sec) c = 'C';
								else c = 'G';
							}
							filters[k].second++;
							detected++;
						}
					}
				}
				result += seq;
				result += '\n';
				seq.clear();
			}
		}
		cout << "\tFiltered Seqences : " << detected << endl;
		string ext;
		while (path[i].back() != '.') {
			ext += path[i].back();
			path[i].pop_back();
		}
		path[i].pop_back();
		reverse(ext.begin(), ext.end());
		path[i] += "_filtered";
		path[i] += ".";
		path[i] += ext;
		FileIO::SaveFileData(path[i], result);
	}
}