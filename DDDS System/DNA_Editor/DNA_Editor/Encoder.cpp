#include <iostream>
#include <ezpwd/rs>
#include "Encoder.h"
#include "FileIO.h"
#include "Config.h"

using namespace std;

int Encoder::Input() {
	/*cout << "Partition : ";
	cin >> this->repetition;*/
	cout << "Repetition : ";
	cin >> this->repetition;
    return 0;
}

int Encoder::Generate(std::vector<std::string> &path) {
	this->path = path;
	this->dna_set.clear();
	cout << "[DNA Strand Generating]" << endl;
	for (int i = 0; i < path.size(); i++) {
		cout << "File " << i + 1 << "/" << path.size() << endl;
		std::string str;
		str.assign(path[i].begin(), path[i].end());
		cout << "\tPath : " << str << endl;
		string dat = FileIO::ReadFileData(str);
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
		vector<DNA> data_dna;
		for (int i = 0; i < partition.size(); i++) {
			string data = partition[i];
			DataRS rs;
			string parity;
			rs.encode(data, parity);
			DNA dna;
			dna.from_binary(data + parity);
			data_dna.push_back(dna);
		}

		int minimum_dna_number = ((double)partition.size() - DATA_OVERLAP) / (DATA_COUNT - DATA_OVERLAP) + 1;
		int dna_number = ceil(this->repetition * (double)partition.size() / DATA_COUNT);
		if(dna_number < minimum_dna_number) dna_number = minimum_dna_number;
		cout << "\tDNA Number : " << dna_number << endl;
		cout << "\tRepetition : " << (double)dna_number * DATA_COUNT / (double)partition.size() << endl;
		
		vector<DNA> dna_set;
		for (int i = 0; i < dna_number; i++) {
			DNA dna;
			int index = i;
			int index_parity = index % 4;
			int size_number = partition.size();
			DNA index_dna, index_parity_dna, size_number_dna;
			index_dna.from_int(index, INDEX_SIZE);
			index_parity_dna.from_int(index_parity, INDEX_PARITY_SIZE);
			size_number_dna.from_int(size_number, INDEX_NUMBER_SIZE);
			dna.insert(dna.end(), index_dna.begin(), index_dna.end());
			dna.insert(dna.end(), index_parity_dna.begin(), index_parity_dna.end());
			dna.insert(dna.end(), size_number_dna.begin(), size_number_dna.end());

			for (int j = 0; j < DATA_COUNT; j++) {
				dna.insert(dna.end(), data_dna[(i * (DATA_COUNT - DATA_OVERLAP) + j) % data_dna.size()].begin(), data_dna[(i * (DATA_COUNT - DATA_OVERLAP) + j) % data_dna.size()].end());
			}
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