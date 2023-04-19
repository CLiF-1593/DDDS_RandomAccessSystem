#include <iostream>
#include <string>
#include <Windows.h>
#include <vector>
using namespace std;

struct DataSize {
	int byte;
	int bit;
	int nucleotide;
};

void GetBinary(string& bin, string& data) {
	int percent = 0;
	for (int i = 0; i < data.size(); i++) {
		char c = data[i];
		if (i * 100 / (data.size() - 1) != percent) {
			percent = i * 100 / (data.size() - 1);
			cout << "\rConvert Data to Binary Type ... (" << percent << "%)";
		}
		for (int j = 7; j >= 0; j--) {
			char unit = 1 << j;
			if (unit & c) {
				bin.push_back('1');
			}
			else {
				bin.push_back('0');
			}
		}
	}
	cout << endl;
}

void GetNucleotide(string& nucleotide, string& bin) {
	int percent = 0;
	if (bin.size() % 2) {
		cout << "!!!ERROR!!!" << endl;
		return;
	}
	for (int i = 0; i < bin.size(); i += 2) {
		if (i * 100 / (bin.size() - 2) != percent) {
			percent = i * 100 / (bin.size() - 2);
			cout << "\rConvert Binary to Nucleotide Sequences ... (" << percent << "%)";
		}
		string dat;
		dat.push_back(bin[i]);
		dat.push_back(bin[i + 1]);
		
		if (dat == "00") {
			nucleotide.push_back('A');
		}
		else if (dat == "01") {
			nucleotide.push_back('C');
		}
		else if(dat == "10") {
			nucleotide.push_back('G');
		}
		else if (dat == "11") {
			nucleotide.push_back('T');
		}
	}

	cout << endl << endl;
}

void Encoding(int seed, string location, string filename, string format, vector<DataSize> &size) {
	FILE* f;
	string file_data;
	DataSize s;
	location += "\\" + filename + "." + format;
	fopen_s(&f, location.c_str(), "rb");
	while (!feof(f)) file_data.push_back(getc(f));
	file_data.pop_back();
	fclose(f);
	cout << endl;
	cout << "[File Information]" << endl;
	cout << "> Location : " << location << endl;
	cout << "> File Name : " << filename << endl;
	cout << "> Format : " << format << endl;
	cout << "> Size : " << file_data.size() * 8 << " bit (" << file_data.size() << " byte)" << endl << endl;

	string seed_bin;
	for (int j = 3; j >= 0; j--) {
		char unit = 1 << j;
		if (unit & seed) {
			seed_bin.push_back('1');
		}
		else {
			seed_bin.push_back('0');
		}
	}

	cout << "[Seed] : " << seed << " " + seed_bin << endl << endl;

	s.byte = file_data.size();

	string binary;
	GetBinary(binary, file_data);
	binary = seed_bin + binary;
	s.bit = binary.size();

	fopen_s(&f, (location + " [binary].txt").c_str(), "wb");
	for (int i = 0; i < binary.size(); i++) {
		fputc(binary[i], f);
	}
	fclose(f);

	string nucleotide;
	GetNucleotide(nucleotide, binary);
	s.nucleotide = nucleotide.size();

	fopen_s(&f, (location + " [nucleotide].txt").c_str(), "wb");
	for (int i = 0; i < nucleotide.size(); i++) {
		fputc(nucleotide[i], f);
	}
	fclose(f);

	cout << "[Encoding Information]" << endl;
	cout << "> Nucleotide : " << nucleotide.size() << endl << endl;
	cout << "[Converting Complete]" << endl << endl;
	cout << "=======================" << endl;
	size.push_back(s);
}

int main() {
	string location;
	cout << "DNA Sequence Encoder" << endl;
	cout << "2023 R&E of KSA" << endl << endl;

	/*cout << "Location : ";
	cin >> location;

	Encoding(location);*/

	vector<DataSize> size;

	for (int i = 0; i < 27; i++) {
		Encoding(i, "D:\\KSA\\R&E\\text", to_string(i), "txt", size);
	}

	DataSize tot = {0,0,0};
	for (int i = 0; i < size.size(); i++) {
		tot.byte += size[i].byte;
		tot.bit += size[i].bit;
		tot.nucleotide += size[i].nucleotide;
	}

	cout << "Total Bytes = " << tot.byte << " Average = " << (double)tot.byte / (double)size.size() << endl;
	cout << "Total Bits = " << tot.bit << " Average = " << (double)tot.bit / (double)size.size() << endl;
	cout << "Total Nucleotides = " << tot.nucleotide << " Average = " << (double)tot.nucleotide / (double)size.size() << endl;
	return 0;
}