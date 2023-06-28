#pragma once
#include <vector>
#include <string>
#include "Config.h"

#define INDEX_SIZE 5
#define INDEX_OVERFLOW_PARM_SIZE 1
#define INDEX_PARITY_SIZE 4
#define DATA_SIZE 12
#define DATA_PARITY_SIZE 8

#define REPETITION 3
#define MAX_INDEX_ORDER 10

void PrintDNA(DNA dna);

class Strand {
private:
	DNA index;
	DNA index_parity;
	DNA data;
	DNA data_parity;

public:
	Strand(int index, int overflow_parameter, std::string data);

	DNA GetIndexSeq();
	DNA GetDataSeq();
};

class DNA_Pool {
private:
	int size;

	std::vector<std::vector<DNA>> dna_sequence; // Store DNA sequences that recovered index completely
	std::vector<std::string> data; // Stroe original datas recoverd perfectly
	
	std::vector<DNA> unknown_sequence;

	int MergeMultipleDNA();
	int ExtractDataFromDNA();

	int fatal_err_cnt;
	int err_cnt;

public:
	DNA_Pool();
	int AppendDNA(DNA strand);
	int Processing();
	std::string GetData();
	int GetSize();
	int GetErrCnt();
	int GetFatalErrCnt();
};

class InsCandidate {
public:
	int frequency;
	int index;
	Nucleotide nucleotide;

	InsCandidate();
	InsCandidate(int frequency, int index, Nucleotide nucleotide);

	bool operator<(const InsCandidate& other) const;
	bool operator>(const InsCandidate& other) const;
};