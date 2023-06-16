#pragma once
#include <vector>
#include <string>
#include "Config.h"

#define STRAND_SIZE 120
#define RE_SIZE 6
#define INDEX_SIZE 5
#define INDEX_OVERFLOW_PARM_SIZE 3
#define INDEX_PARITY_SIZE 4
#define DATA_SIZE 16
#define DATA_PARITY_SIZE 8

#define REPETITION 4
#define MAX_INDEX_ORDER 10

void PrintDNA(DNA dna);

class Strand {
private:
	DNA restriction_enzyme;
	DNA index;
	DNA index_parity;
	DNA data;
	DNA data_parity;

public:
	Strand(int index, int overflow_parameter, std::string data, std::vector<Nucleotide> restriction_enzyme);

	DNA GetRestrictionEnzymeSeq();
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