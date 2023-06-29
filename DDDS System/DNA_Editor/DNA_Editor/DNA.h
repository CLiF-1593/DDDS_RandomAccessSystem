#pragma once
#include <vector>
#include <string>
#include <ezpwd/rs>
#include "Config.h"


#define INDEX_SIZE 5
#define INDEX_PARITY_SIZE 1
#define INDEX_NUMBER_SIZE 4

#define DATA_SIZE 12
#define DATA_PARITY_SIZE 8

#define DATA_COUNT 3
#define DATA_OVERLAP 0

#define CANDIDATE_RATIO 0.3
#define MAXIMUM_CALCULATING_TIME 30000

using DataRS = ezpwd::RS<255, 255 - BPSToByte(DATA_PARITY_SIZE)>;

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

class DNA : public std::vector<Nucleotide> {
public:
	DNA slice(int begin, int end);
	void ins(int index, Nucleotide nucleotide);
	Nucleotide del(int index);

	std::string to_binary();
	unsigned int to_int();
	void from_binary(std::string binary);
	void from_int(int integer, int size);

	std::string str();
};

using DNA_Set = std::vector<DNA>;

class DNA_Classifier {
private:
	std::vector<DNA_Set> dna_pool;
	std::vector<DNA> unknown_dna_pool;
	std::vector<std::pair<int,int>> size_candidate;
	int size;
	int total_append_num;

public:
	DNA_Classifier();
	int Append(DNA dna);
	DNA_Set GetDataSet(int index);
	int GetSize();
	int GetNumber_Unknown();
	int GetNumber_Total();

	void MergeDNA();
};

class DNA_Analyzer {
private:
	
	DNA_Set *dna_set;
	DNA base_dna;
	std::vector<InsCandidate> ins_candidate;

	std::string result;

	void SetEditDistance();
	void SetCandidate();
	void SetCorrectDNA();

	int missing;

public:
	void InitDNA(DNA_Set *dna_set);
	void Analyze();
	std::string GetData();
	std::string GetLowQualityData();
};