#include "Encoder.h"
#include <iostream>
#include "FileIO.h"
using namespace std;

int Encoder::Input() {
	cout << "Partition : ";
	cin >> this->partition;
	cout << "Repetition : ";
	cin >> this->repetition;
    return 0;
}

int Encoder::Generate(std::vector<std::wstring> *path) {
	this->Read(path);

	return 0;
}

void Encoder::Read(std::vector<std::wstring>* path) {
	this->binary_data.clear();
	for (int i = 0; i < path->size(); i++) {
		std::string str;
		str.assign((*path)[i].begin(), (*path)[i].end());
		this->binary_data.push_back(FileIO::ReadFileData(str));
	}
}

void Encoder::ConvertRawDNA() {
	for (int i = 0; i < this->binary_data.size(); i++) {
		string dna = "";
		for (int j = 0; j < this->binary_data[i].size(); i++) {
			string section = "";
			char c = this->binary_data[i][j];
			for (int k = 0; k < 4; k++) {
				switch (c & 3) {
				case 0: section += "A"; break;
				case 1: section += "G"; break;
				case 2: section += "C"; break;
				case 3: section += "T"; break;
				}
				c >>= 2;
			}
			section.reserve();
			dna += section;
		}
		this->raw_dna.push_back(dna);
	}
}