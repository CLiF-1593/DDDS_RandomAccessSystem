#include "Convertor.h"
#include <iostream>
using namespace std;

std::string Convertor::BinToDna(std::string dat) {
	std::string ret;
	for (int i = 0; i < dat.size(); i++) {
		std:: string section = "";
		char c = dat[i];
		for (int k = 0; k < 4; k++) {
			switch (c & 3) {
			case 0: section += "A"; break;
			case 1: section += "G"; break;
			case 2: section += "C"; break;
			case 3: section += "T"; break;
			}
			c >>= 2;
		}
		reverse(section.begin(), section.end());
		ret += section;
	}
    return ret;
}

std::string Convertor::DnaToBin(std::string dna) {
	std::string ret;
	for (int i = 0; i < dna.size(); i += 4) {
		char c = 0;
		for (int j = 0; j < 4; j++) {
			c <<= 2;
			if (dna[i + j] == 'A') c += 0;
			else if (dna[i + j] == 'G') c += 1;
			else if (dna[i + j] == 'C') c += 2;
			else if (dna[i + j] == 'T') c += 3;
		}
		ret += c;
	}
	return ret;
}
