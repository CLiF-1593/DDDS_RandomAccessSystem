#include <iostream>
#include <ezpwd/rs>
#include "Encoder.h"
#include "FileIO.h"
#include "Config.h"
#include "EncodingTable.h"

using namespace std;

int Encoder::Input() {
	/*cout << "Partition : ";
	cin >> this->repetition;*/
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

		cout << "\tEncoding Type" << endl;
		cout << "\t\t0 : Standard Encoding (ASCII)" << endl;
		cout << "\t\t1 : English Custom Table Encoding" << endl;
		cout << "\t\t2 : All Language Custom Table Encoding" << endl;
		cout << "\t\t3 : Coordinate (Integer)" << endl << "\t\t";
		int encoding_type;
		cin >> encoding_type;

		string dna_dat = Encoding(static_cast<EncodingType>(encoding_type), dat);
		while (dna_dat.size() < 12) dna_dat.push_back(0);
		dna_dat.resize(12);

		DataRS rs;
		string parity;
		rs.encode(dna_dat, parity);

		cout << "\tGenerating DNA Strands . . ." << endl;
		DNA dna;
		dna.from_binary(dna_dat + parity);
		this->dna_set.push_back(dna);

		cout << "\tGenerated" << endl;
		cout << "\tNumber of DNA Strands : " << dna.size() << endl << endl;
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
			switch ((4 * j + (int)(this->dna_set[i][j]) - j) % 4) {
			case 0: dat += "A"; break;
			case 1: dat += "G"; break;
			case 2: dat += "T"; break;
			case 3: dat += "C"; break;
			}
		}
		if (dat.size()) dat.pop_back();
		FileIO::SaveFileData(str, dat);
	}
	return 0;
}