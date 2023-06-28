#include "Convertor.h"
#include <iostream>
using namespace std;

DNA Convertor::BinToDna(std::string dat) {
	DNA ret;
	for (int i = 0; i < dat.size(); i++) {
		DNA section;
		char c = dat[i];
		for (int k = 0; k < 4; k++) {
			section.push_back(static_cast<Nucleotide>(c & 3));
			c >>= 2;
		}
		reverse(section.begin(), section.end());
		ret.insert(ret.end(), section.begin(), section.end());
	}
    return ret;
}

std::string Convertor::DnaToBin(DNA dna) {
	std::string ret;
	while (dna.size() & 3) {
		dna.push_back(A);
	}
	for (int i = 0; i < dna.size(); i += 4) {
		char c = 0;
		for (int j = 0; j < 4; j++) {
			c <<= 2;
			c += dna[i + j];
		}
		ret += c;
	}
	return ret;
}
