#pragma once
#include "Generator.h"
#include <vector>

class Encoder : public Generator {
public:
	int Input() override;
	int Generate(std::vector<std::wstring> *path) override;
	int Output() override;

private:
	int partition;
	int repetition;
	std::vector<std::wstring> path;

	std::vector<std::string> binary_data;
	std::vector<std::string> raw_dna;
	std::vector<std::vector<std::string>> dna_seqence;

	void Read(std::vector<std::wstring>* path);
	void ConvertRawDNA();
	void Partitioning();
	void AddReedSolomonCode();
	void Write();
};