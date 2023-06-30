#include "Decoder.h"
#include "FileIO.h"
#include "Config.h"
#include <iostream>
#include <ezpwd/rs>
#include "EncodingTable.h"
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

		cout << "\tEncoding Type" << endl;
		cout << "\t\t0 : Standard Encoding (ASCII)" << endl;
		cout << "\t\t1 : English Custom Table Encoding" << endl;
		cout << "\t\t2 : All Language Custom Table Encoding" << endl;
		cout << "\t\t3 : Coordinate (Integer)" << endl << "\t\t";
		int encoding_type;
		cin >> encoding_type;

		cout << "\tTable Type (0 : std) : ";
		int table_type = 0;
		if(encoding_type != COORDINATE && encoding_type != ASCII)
			cin >> table_type;

		cout << "\tSeparating DNA Strands . . ." << endl;
		vector<DNA> dna_set;
		DNA push_dna;
		int cnt = 0;
		for (int i = 0; i < dat.size(); i++) {
			if (dat[i] == 'A') push_dna.push_back(static_cast<Nucleotide>(cnt % 4));
			else if (dat[i] == 'G') push_dna.push_back(static_cast<Nucleotide>((cnt + 1) % 4));
			else if (dat[i] == 'T') push_dna.push_back(static_cast<Nucleotide>((cnt + 2) % 4));
			else if (dat[i] == 'C') push_dna.push_back(static_cast<Nucleotide>((cnt + 3) % 4));
			else if (dat[i] == '\n') {
				dna_set.push_back(push_dna);
				push_dna.clear();
				cnt = -1;
			}
			cnt++;
		}
		if (push_dna.size()) {
			dna_set.push_back(push_dna);
			push_dna.clear();
		}
		cout << "\tNumber of DNA Strands : " << dna_set.size() << endl;
		//classifier.MergeDNA();
		cout << "\tAnalyzing . . ." << endl;
		DNA_Analyzer analyzer;
		int err_cnt = 0;
		int fatal_err_cnt = 0;
		int missing_cnt = 0;
		analyzer.InitDNA(&dna_set);
		analyzer.Analyze();
		string missing_data = dna_set[0].to_binary();
		string data = analyzer.GetData();
		if (data.empty()) {
			data = analyzer.GetLowQualityData();
			if (data.empty()) {
				if(dna_set.empty()) missing_cnt++;
				else {
					data = missing_data;
					fatal_err_cnt++;
				}
			}
			else err_cnt++;
		}
		cout << endl;
		cout << "\tError Correcting and Extracting Datas from DNA Strands" << endl;
		cout << "\tCompleted" << endl;
		if (err_cnt) cout << "\tError Occured" << endl;
		else if (fatal_err_cnt) cout << "\tFatal Error Occured" << endl;
		else if (missing_cnt) cout << "\tMissing" << endl;
		else cout << "\tSuccess" << endl << endl;

		data = Decoding(static_cast<EncodingType>(encoding_type), table_type, data);

		this->result.push_back(data);
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