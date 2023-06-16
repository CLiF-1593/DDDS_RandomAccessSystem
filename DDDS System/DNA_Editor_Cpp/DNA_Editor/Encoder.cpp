#include <iostream>
#include <ezpwd/rs>
#include "Encoder.h"
#include "FileIO.h"
#include "Convertor.h"
#include "Config.h"

using namespace std;

int Encoder::Input() {
	/*cout << "Partition : ";
	cin >> this->partition;
	cout << "Repetition : ";
	cin >> this->repetition;*/
    return 0;
}

int Encoder::Generate(std::vector<std::wstring> &path) {
	this->path = path;
	this->dna_set.clear();
	cout << "[DNA Strand Generating]" << endl;
	for (int i = 0; i < path.size(); i++) {
		cout << "File " << i + 1 << "/" << path.size() << endl;
		std::string str;
		str.assign(path[i].begin(), path[i].end());
		cout << "\tPath : " << str << endl;
		string dat = FileIO::ReadFileData(str);
		cout << "\tSet Restriction Enzyme Recognition Site" << endl;
		std::wstring re_seq_str;
		int re_index = path[i].size() - 1;
		while (path[i][re_index] != L'.') re_index--;
		re_index--;
		while (path[i][re_index] != L'.') {
			re_seq_str += path[i][re_index];
			re_index--;
		}
		DNA re_seq;
		for (int i = re_seq_str.size() - 1; i >= 0; i--) {
			if (re_seq_str[i] == L'A') re_seq.push_back(A);
			else if (re_seq_str[i] == L'G') re_seq.push_back(G);
			else if (re_seq_str[i] == L'C') re_seq.push_back(C);
			else if (re_seq_str[i] == L'T') re_seq.push_back(T);
		}
		int re_plus = 0;
		while (re_seq.size() < RE_SIZE) {
			re_seq.push_back(static_cast<Nucleotide>(re_plus & 3));
			re_plus++;
		}
		re_seq.resize(RE_SIZE);
		cout << "\tRestriction Enzyme Recognition Site : ";
		wcout << re_seq_str << endl;
		cout << "\tPartition Size : " << DATA_SIZE << "nt (" << (int)(BPSToByte(DATA_SIZE)) << "byte)" << endl;
		cout << "\tPartitioning . . ." << endl;
		vector<string> partition;
		string part;
		for (int i = 0; i < dat.size(); i++) {
			part += dat[i];
			if (part.size() == BPSToByte(DATA_SIZE)) {
				partition.push_back(part);
				part.clear();
			}
		}
		if (part.size()) {
			part.resize(BPSToByte(DATA_SIZE));
			partition.push_back(part);
		}
		cout << "\tGenerating DNA Strands . . ." << endl;
		vector<Strand> strand_set;
		for (int i = 0; i < partition.size(); i++) {
			int overflow_parm = 0;
			if (i > partition.size() - REPETITION) overflow_parm = REPETITION - partition.size() + i;
			Strand strand(i, overflow_parm, partition[i], re_seq);
			strand_set.push_back(strand);
		}
		vector<DNA> dna_set;
		for (int i = 0; i < strand_set.size(); i++) {
			DNA re_dna = strand_set[i].GetRestrictionEnzymeSeq();
			DNA index_dna = strand_set[i].GetIndexSeq();
			DNA data_dna = strand_set[i].GetDataSeq();
			DNA overlap_data1_dna = strand_set[(i + 1) % strand_set.size()].GetDataSeq();
			DNA overlap_data2_dna = strand_set[(i + 2) % strand_set.size()].GetDataSeq();
			DNA overlap_data3_dna = strand_set[(i + 3) % strand_set.size()].GetDataSeq();
			DNA dna;
			dna.insert(dna.end(), re_dna.begin(), re_dna.end());
			dna.insert(dna.end(), index_dna.begin(), index_dna.end());
			dna.insert(dna.end(), data_dna.begin(), data_dna.end());
			dna.insert(dna.end(), overlap_data1_dna.begin(), overlap_data1_dna.end());
			dna.insert(dna.end(), overlap_data2_dna.begin(), overlap_data2_dna.end());
			dna.insert(dna.end(), overlap_data3_dna.begin(), overlap_data3_dna.end());
			reverse(re_dna.begin(), re_dna.end());
			dna.insert(dna.end(), re_dna.begin(), re_dna.end());
			dna_set.push_back(dna);
		}
		this->dna_set.push_back(dna_set);
		cout << "\tGenerated" << endl;
		cout << "\tNumber of DNA Strands : " << dna_set.size() << endl << endl;
	}
	return 0;
}

int Encoder::Output() {
	cout << "[Save Data]" << endl;
	for (int i = 0; i < this->path.size(); i++) {
		cout << "File " << i + 1 << "/" << path.size();
		std::string str;
		str.assign(path[i].begin(), path[i].end());
		str += ".dna";
		cout << "\tPath : " << str << endl;
		string dat;
		for (int j = 0; j < this->dna_set[i].size(); j++) {
			for (int k = 0; k < this->dna_set[i][j].size(); k++) {
				switch (this->dna_set[i][j][k]) {
				case A: dat += "A"; break;
				case G: dat += "G"; break;
				case C: dat += "C"; break;
				case T: dat += "T"; break;
				}
			}
			dat += "\n";
		}
		if (dat.size()) dat.pop_back();
		FileIO::SaveFileData(str, dat);
	}
	return 0;
}