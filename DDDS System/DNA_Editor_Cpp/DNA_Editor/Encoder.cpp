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

int Encoder::Generate(std::vector<std::wstring> *path) {
	this->Read(path);
	this->ConvertRawDNA();
	this->Partitioning();
	this->AddReedSolomonCode();
	return 0;
}

int Encoder::Output() {
	this->Write();
	return 0;
}

void Encoder::Read(std::vector<std::wstring>* path) {
	this->path = *path;
	this->binary_data.clear();
	cout << "> Reading . . ." << endl;
	for (int i = 0; i < path->size(); i++) {
		std::string str;
		str.assign((*path)[i].begin(), (*path)[i].end());
		cout << "\t" << str << endl;
		this->binary_data.push_back(FileIO::ReadFileData(str));
	}
	cout << "> Reading Completed" << endl << endl;
}

void Encoder::ConvertRawDNA() {
	this->raw_dna.clear();
	cout << "> Converting to Raw DNA . . ." << endl;
	for (int i = 0; i < this->binary_data.size(); i++) {
		cout << "\t" << "File Num : " << i << endl;
		string dna = Convertor::BinToDna(this->binary_data[i]);
		this->raw_dna.push_back(dna);
	}
	cout << "> Converting Completed" << endl << endl;
	this->binary_data.clear();
}

void Encoder::Partitioning() {
	cout << "> Partitioning . . ." << endl;
	this->dna_seqence.clear();
	for (int i = 0; i < this->raw_dna.size(); i++) {
		cout << "\t" << "File Num : " << i << endl;
		vector<string> dna;
		string part;
		for (int j = 0; j < this->raw_dna[i].size(); j++) {
			part += this->raw_dna[i][j];
			if (part.size() >= DNA_LEN - PARITY_SIZE) {
				dna.push_back(part);
				part.clear();
			}
		}
		if (!part.empty()) {
			dna.push_back(part);
		}
		this->dna_seqence.push_back(dna);
	}	
	this->raw_dna.clear();
	cout << "> Partitioning Completed" << endl << endl;
}

void Encoder::AddReedSolomonCode() {
	cout << "> Adding Reed Solomon Code . . ." << endl;
	ezpwd::RS<255, 255 - (char)BPSToByte(PARITY_SIZE)> rs;
	for (int i = 0; i < this->dna_seqence.size(); i++) {
		cout << "\t" << "File Num : " << i << endl;
		for (int j = 0; j < this->dna_seqence[i].size(); j++) {
			string seq = this->dna_seqence[i][j];
			string data = Convertor::DnaToBin(seq);
			string parity;
			rs.encode(data, parity);
			data = Convertor::BinToDna(data);
			parity = Convertor::BinToDna(parity);
			this->dna_seqence[i][j] = data + "" + parity;
		}
	}
	cout << "> Adding Reed Solomon Code Completed" << endl << endl;
}

void Encoder::Write() {
	cout << "> Writing . . ." << endl;
	for (int i = 0; i < this->path.size() && i < this->dna_seqence.size(); i++) {
		wstring save_path = this->path[i] + L".dna";
		std::string str;
		str.assign(save_path.begin(), save_path.end());
		cout << "\t" << str << endl;
		string dat = "";
		for (int j = 0; j < this->dna_seqence[i].size(); j++) {
			dat += this->dna_seqence[i][j];
			dat.push_back('\n');
		}
		FileIO::SaveFileData(str, dat);
	}
	cout << "> Writing Completed" << endl << endl;
}
