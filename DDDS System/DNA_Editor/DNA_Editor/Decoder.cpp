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

int Decoder::Generate(std::vector<std::wstring> *path) {
	this->Read(path);
	this->DecodeReedSolomonCode();
	this->Combining();
	this->ConvertBinary();
	return 0;
}

int Decoder::Output() {
	this->Write();
	return 0;
}

void Decoder::Read(std::vector<std::wstring>* path) {
	this->path = *path;
	this->dna_seqence.clear();
	cout << "> Reading . . ." << endl;
	for (int i = 0; i < path->size(); i++) {
		std::string str;
		str.assign((*path)[i].begin(), (*path)[i].end());
		cout << "\t" << str << endl;
		string dat = FileIO::ReadFileData(str);
		vector<string> vec;
		string seq = "";
		for (int i = 0; i < dat.size(); i++) {
			if (dat[i] == '\n') {
				vec.push_back(seq);
				seq.clear();
				continue;
			}
			seq += dat[i];
		}
		this->dna_seqence.push_back(vec);
	}
	cout << "> Reading Completed" << endl << endl;
}

void Decoder::DecodeReedSolomonCode() {
	cout << "> Decoding Reed Solomon Code . . ." << endl;
	ezpwd::RS<255, 255 - (char)BPSToByte(PARITY_SIZE)> rs;
	for (int i = 0; i < this->dna_seqence.size(); i++) {
		cout << "\t" << "File Num : " << i << endl;
		for (int j = 0; j < this->dna_seqence[i].size(); j++) {
			string bin = Convertor::DnaToBin(this->dna_seqence[i][j]);
			rs.decode(bin);
			this->dna_seqence[i][j] = Convertor::BinToDna(bin);
		}
	}
	cout << "> Decoding Reed Solomon Completed" << endl << endl;
}

void Decoder::Combining() {
	cout << "> Combining . . ." << endl;
	this->raw_dna.clear();
	for (int i = 0; i < this->dna_seqence.size(); i++) {
		cout << "\t" << "File Num : " << i << endl;
		string str = "";
		for (int j = 0; j < this->dna_seqence[i].size(); j++) {
			str += this->dna_seqence[i][j];
		}
		this->raw_dna.push_back(str);
	}
	this->dna_seqence.clear();
	cout << "> Combining Completed" << endl << endl;
}

void Decoder::ConvertBinary() {
	cout << "> Converting DNA sequences to binary data . . ." << endl;
	this->binary_data.clear();
	for (int i = 0; i < this->raw_dna.size(); i++) {
		cout << "\t" << "File Num : " << i << endl;
		this->binary_data.push_back(Convertor::DnaToBin(this->raw_dna[i]));
	}
	this->raw_dna.clear();
	cout << "> Converting Completed" << endl << endl;
}

void Decoder::Write() {
	cout << "> Writing . . ." << endl;
	for (int i = 0; i < this->path.size() && i < this->binary_data.size(); i++) {
		wstring save_path = this->path[i] + L".origin";
		std::string str;
		str.assign(save_path.begin(), save_path.end());
		cout << "\t" << str << endl;
		FileIO::SaveFileData(str, this->binary_data[i]);
	}
	cout << "> Writing Completed" << endl << endl;
}
