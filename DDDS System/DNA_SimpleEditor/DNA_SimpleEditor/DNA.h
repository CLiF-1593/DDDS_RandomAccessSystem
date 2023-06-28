#pragma once
#include <vector>
#include <string>
#include <ezpwd/rs>
#include "Config.h"

#define DATA_SIZE 48
#define DATA_PARITY_SIZE 20

#define CANDIDATE_RATIO 0.3
#define MAXIMUM_CALCULATING_TIME 15000

using DataRS = ezpwd::RS<255, 255 - 5>;

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