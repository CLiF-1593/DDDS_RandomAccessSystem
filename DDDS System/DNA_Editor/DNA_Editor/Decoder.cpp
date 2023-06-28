#include "Decoder.h"
#include "FileIO.h"
#include "Config.h"
#include <iostream>
#include <ezpwd/rs>
using namespace std;

int Decoder::Input() {
    return 0;
}

int Decoder::Generate(std::vector<std::string> &path) {
	this->path = path;
	this->result.clear();
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
		cout << "\tClassifying . . ." << endl;
		DNA_Classifier classifier;
		int fail_cnt = 0;
		for (int i = 0; i < dna_set.size(); i++) {
			fail_cnt += IsFailed(classifier.Append(dna_set[i]));
		}
		//classifier.MergeDNA();
		cout << "\tAnalyzing . . ." << endl;
		DNA_Analyzer analyzer;
		int err_cnt = 0;
		int fatal_err_cnt = 0;
		int missing_cnt = 0;
		string result;
		for (int i = 0; i < classifier.GetSize(); i++) {
			DNA_Set set = classifier.GetDataSet(i);
			analyzer.InitDNA(&set);
			analyzer.Analyze();
			string data = analyzer.GetData();
			if (data.empty()) {
				data = analyzer.GetLowQualityData();
				if (data.empty()) {
					if(set.empty()) missing_cnt++;
					else {
						data = classifier.GetDataSet(i)[0].to_binary();
						fatal_err_cnt++;
					}
				}
				else err_cnt++;
			}
			data.resize(BPSToByte(DATA_SIZE));
			result += data;
		}
		cout << endl;
		cout << "\tSuccess : " << classifier.GetNumber_Total() - classifier.GetNumber_Unknown() << " / " << classifier.GetNumber_Total() << endl;
		cout << "\tError Correcting and Extracting Datas from DNA Strands" << endl;
		cout << "\tCompleted" << endl;
		cout << "\tTotal Data part Number : " << classifier.GetSize() << endl;
		cout << "\tError DNA part Number : " << err_cnt << endl;
		cout << "\tFatal Error DNA part Number : " << fatal_err_cnt << endl;
		cout << "\tMissing DNA part Number : " << missing_cnt << endl;
		this->result.push_back(result);
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
		FileIO::SaveFileData(str, this->result[i]);
	}
	return 0;
}