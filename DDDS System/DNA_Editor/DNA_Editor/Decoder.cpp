#include "Decoder.h"
#include "FileIO.h"
#include "Config.h"
#include "Convertor.h"
#include <iostream>
#include <ezpwd/rs>
using namespace std;

int Decoder::Input() {
    return 0;
}

int Decoder::Generate(std::vector<std::wstring> &path) {
	this->path = path;
	cout << "[DNA Data Decoding]" << endl;
	for (int i = 0; i < path.size(); i++) {
		cout << "File " << i + 1 << "/" << path.size();
		std::string str;
		str.assign(path[i].begin(), path[i].end());
		cout << "\tPath : " << str << endl;
		string dat = FileIO::ReadFileData(str);
		cout << "\tSeparating DNA Strands . . ." << endl;
		vector<DNA> dna_set;
		DNA push_dna;
		for (int i = 0; i < dat.size(); i++) {
			if (dat[i] == 'A') push_dna.push_back(A);
			else if (dat[i] == 'G') push_dna.push_back(G);
			else if (dat[i] == 'C') push_dna.push_back(C);
			else if (dat[i] == 'T') push_dna.push_back(T);
			else if (dat[i] == '\n') {
				dna_set.push_back(push_dna);
				push_dna.clear();
			}
		}
		if (push_dna.size()) {
			dna_set.push_back(push_dna);
			push_dna.clear();
		}
		cout << "\tNumber of DNA Strands : " << dna_set.size() << endl;
		cout << "\tIndexing . . .";
		DNA_Pool dna_pool;
		int fail_cnt = 0;
		for (int i = 0; i < dna_set.size(); i++) {
			fail_cnt += IsFailed(dna_pool.AppendDNA(dna_set[i]));
		}
		cout << "\tSuccess : " << dna_set.size() - fail_cnt << " / " << dna_set.size() << endl;
		cout << "\tError Correcting and Extracting Datas from DNA Strands" << endl;
		dna_pool.Processing();
		cout << "\tCompleted" << endl;
		cout << "\tTotal DNA Strands Number : " << dna_pool.GetSize() << endl;
		cout << "\tError Count : " << dna_pool.GetErrCnt() << endl;
		cout << "\tFatal Error Count : " << dna_pool.GetFatalErrCnt() << endl << endl;
		this->dna_pool_set.push_back(dna_pool);
	}
	return 0;
}

int Decoder::Output() {
	cout << "[Save Data]" << endl;
	for (int i = 0; i < this->path.size(); i++) {
		cout << "File " << i + 1 << "/" << path.size();
		std::string str;
		str.assign(path[i].begin(), path[i].end());
		str += ".origin";
		cout << "\tPath : " << str << endl;
		string dat;
		FileIO::SaveFileData(str, this->dna_pool_set[i].GetData());
	}
	return 0;
}