#include "DNA.h"
#include <ezpwd/rs>
#include <assert.h>
#include "Convertor.h"
#include "DeltaDNA.h"
using namespace std;

using IndexRS = ezpwd::RS<255, 255 - BPSToByte(INDEX_PARITY_SIZE)>;
using DataRS = ezpwd::RS<255, 255 - BPSToByte(DATA_PARITY_SIZE)>;

#define MIN(x,y) ((x) < (y) ? (x) : (y))

void PrintDNA(DNA dna) {
	for (int i = 0; i < dna.size(); i++) {
		switch (dna[i]) {
		case A: std::cout << "A"; break;
		case G: std::cout << "G"; break;
		case C: std::cout << "C"; break;
		case T: std::cout << "T"; break;
		case X: std::cout << "_"; break;
		}
	}
	std::cout << std::endl;
}



Strand::Strand(int index, int overflow_parameter, std::string data, std::vector<Nucleotide> restriction_enzyme) {
	assert(index >= 0 || index < pow(2, MAX_INDEX_ORDER)); // 0~2^MAX_INDEX_ORDER
	assert(overflow_parameter >= 0 || overflow_parameter < REPETITION); // 0~2^MAX_INDEX_ORDER
	assert(data.size() == BPSToByte(DATA_SIZE));

	// Setting Restriction Enzyme Sequences
	this->restriction_enzyme = restriction_enzyme;
	
	// Setting Index Sequences
	IndexRS index_rs;
	string index_data, index_parity;
	index_data.push_back((char)(index >> (MAX_INDEX_ORDER - 8)));
	index_data.push_back(0);
	for (int i = 0; i < (8 - (MAX_INDEX_ORDER - 8)) / 2; i++) {
		index_data.back() <<= 2;
		index_data.back() += (overflow_parameter + i) & 3;
	}
	index_data.back() += (unsigned char)(index << 8 - (MAX_INDEX_ORDER - 8));
	index_rs.encode(index_data, index_parity);
	this->index = Convertor::BinToDna(index_data);
	this->index_parity = Convertor::BinToDna(index_parity);

	// Setting Data Sequences
	DataRS data_rs;
	string data_parity;
	data_rs.encode(data, data_parity);
	this->data = Convertor::BinToDna(data);
	this->data_parity = Convertor::BinToDna(data_parity);
}

DNA Strand::GetRestrictionEnzymeSeq() {
	return this->restriction_enzyme;
}

DNA Strand::GetIndexSeq() {
	DNA ret = this->index;
	ret.insert(ret.end(), this->index_parity.begin(), this->index_parity.end());
	return ret;
}

DNA Strand::GetDataSeq() {
	DNA ret = this->data;
	ret.insert(ret.end(), this->data_parity.begin(), this->data_parity.end());
	return ret;
}

DNA Merge(vector<DNA> dna_set) {
	DNA final_dna;
	for (int i = 0; i < dna_set.size() - 1; i++) {
		int index = i;
		int prev_size = 0;
		do {
			prev_size = dna_set[index].size();
			GetDelta(dna_set[i], dna_set[i + 1]);
		} while (prev_size != dna_set[index].size() && index--);
	}
	for (int i = 0; i < dna_set[0].size(); i++) {
		vector<pair<int, Nucleotide>> stack;
		for (int j = 0; j < dna_set.size(); j++) {
			Nucleotide push = dna_set[j][i];
			bool pass = false;
			for (int k = 0; k < stack.size(); k++) {
				if (stack[k].second == push) {
					stack[k].first--;
					pass = true;
					break;
				}
			}
			if (!pass) stack.push_back(make_pair(0, push));
		}
		std::sort(stack.begin(), stack.end());
		final_dna.push_back(stack.front().second);
	}
	for (int i = 0; i < final_dna.size(); i++) {
		if (final_dna[i] == X) {
			final_dna.erase(final_dna.begin() + i);
			i--;
		}
	}
	return final_dna;
}

int DNA_Pool::MergeMultipleDNA() {
	this->dna_sequence.resize(this->size);
	for (int dna_num = 0; dna_num < this->dna_sequence.size(); dna_num++) {
		DNA final_dna;
		auto dna_set = this->dna_sequence[dna_num];
		this->dna_sequence[dna_num].clear();
		if (dna_set.empty()) continue;
		final_dna = Merge(dna_set);
		this->dna_sequence[dna_num].push_back(final_dna);
	}
	return 0;
}

int DNA_Pool::ExtractDataFromDNA() {
	this->data.clear();
	this->data.resize(this->size);
	for (int dna_num = 0; dna_num < this->dna_sequence.size(); dna_num++) {
		// Select 4 Data Strands
		vector<DNA> data_strand;
		for (int i = 0; i < REPETITION; i++) {
			int index = (dna_num + this->dna_sequence.size() - i) % this->dna_sequence.size();
			if (this->dna_sequence[index].size()) {
				DNA dna = this->dna_sequence[index][0];
				DNA data_st;
				auto begin = RE_SIZE + INDEX_SIZE + INDEX_OVERFLOW_PARM_SIZE + INDEX_PARITY_SIZE;
				data_st.insert(data_st.end(), dna.begin() + begin + (DATA_SIZE + DATA_PARITY_SIZE) * i, dna.begin() + MIN(begin + (DATA_SIZE + DATA_PARITY_SIZE) * (i + 1), dna.size()));
				data_strand.push_back(data_st);
			}
		}
		if (data_strand.empty()) {
			this->data[dna_num] = "";
			this->fatal_err_cnt++;
			continue;
		}
		DataRS rs;
		string data;

		// Step 1 : Decode First Strand
 		DNA main_dna = data_strand.front();
		data = Convertor::DnaToBin(main_dna);
		int err = rs.decode(data);
		if (!IsFailed(err)) {
			this->data[dna_num] = data;
			continue;
		}

		// Step 2 : Merge DNAs and Decode
		DNA merged_dna = Merge(data_strand);
		data = Convertor::DnaToBin(merged_dna);
		err = rs.decode(data);
		if (!IsFailed(err)) {
			this->data[dna_num] = data;
			continue;
		}

		// Step 3 : Brute Force (Insertion and Deletion)
		vector<pair<int, Nucleotide>> ins;
		vector<pair<int, Nucleotide>> del;
		vector<pair<int, Nucleotide>> sub;
		for (int i = 1; i < data_strand.size(); i++) {
			DNA dna1 = data_strand.front();
			DNA dna2 = data_strand[i];
			GetDelta(dna1, dna2);
			int index = 0;
			for (int i = 0; i < dna1.size(); i++) {
				if (dna2[i] == X) del.push_back(make_pair(index, X));
				else if (dna1[i] == X) ins.push_back(make_pair(index, dna2[i]));
				else if (dna1[i] != dna2[i]) sub.push_back(make_pair(-index - 1, dna2[i]));
				if(dna1[i] != X) index++;
			}
		}
		ins.erase(unique(ins.begin(), ins.end()), ins.end());
		del.erase(unique(del.begin(), del.end()), del.end());
		vector<pair<int, Nucleotide>> fix = ins;
		fix.insert(fix.end(), del.begin(), del.end());
		std::sort(fix.begin(), fix.end());
		vector<bool> fix_selector;
		fix_selector.resize(fix.size());
		for (int i = 0; i < fix_selector.size(); i++) {
			fix_selector[i] = false;
		}
		if (fix_selector.size()) fix_selector.front() = true;
		bool success = false;
		while (true) {
			DNA strand = data_strand.front();
			int offset = 0;
			for (int i = 0; i < fix_selector.size(); i++) {
				if (fix_selector[i]) {
					if (fix[i].second == X) {
						if (strand.size() <= fix[i].first + offset || fix[i].first + offset < 0) {
							continue;
						}
						for (int j = fix[i].first + offset; j < strand.size() - 1; j++) {
							strand[j] = strand[j + 1];
						}
						strand.pop_back();
						offset--;
					}
					else {
						if (strand.size() < fix[i].first + offset || fix[i].first + offset < 0) {
							continue;
						}
						if (strand.size() == fix[i].first + offset) {
							strand.push_back(fix[i].second);
							continue;
						}
						strand.push_back(X);
						for (int j = strand.size() - 1; j > fix[i].first + offset; j--) {
							strand[j] = strand[j - 1];
						}
						strand[fix[i].first + offset] = fix[i].second;
						offset++;
					}
				}
			}
			if (strand.size() == DATA_SIZE + DATA_PARITY_SIZE) {
				data = Convertor::DnaToBin(strand);
				err = rs.decode(data);
				if (!IsFailed(err)) {
					this->data[dna_num] = data;
					success = true;
					break;
				}
			}
			int pos = 0;
			while (pos < fix_selector.size() && fix_selector[pos]) {
				fix_selector[pos] = false;
				pos++;
			}
			if (pos == fix_selector.size()) {
				break;
			}
			fix_selector[pos] = true;
		}
		if (success) {
			continue;
		}

		// Step 4 : Brute Force (All Cases)
		fix.insert(fix.end(), sub.begin(), sub.end());
		std::sort(fix.begin(), fix.end());
		fix_selector.resize(fix.size());
		for (int i = 0; i < fix_selector.size(); i++) {
			fix_selector[i] = false;
		}
		if (fix_selector.size()) fix_selector.front() = true;
		success = false;
		while (true) {
			DNA strand = data_strand.front();
			int offset = 0;
			for (int i = 0; i < fix_selector.size(); i++) {
				if (fix_selector[i]) {
					if (fix[i].second == X) {
						if (strand.size() <= fix[i].first + offset || fix[i].first + offset < 0) {
							continue;
						}
						for (int j = fix[i].first + offset; j < strand.size() - 1; j++) {
							strand[j] = strand[j + 1];
						}
						strand.pop_back();
						offset--;
					}
					else {
						if (fix[i].first >= 0) {
							if (strand.size() < fix[i].first + offset || fix[i].first + offset < 0) {
								continue;
							}
							if (strand.size() == fix[i].first + offset) {
								strand.push_back(fix[i].second);
								continue;
							}
							strand.push_back(X);
							for (int j = strand.size() - 1; j > fix[i].first + offset; j--) {
								strand[j] = strand[j - 1];
							}
							strand[fix[i].first + offset] = fix[i].second;
							offset++;
						}
						else {
							int first = -fix[i].first - 1;
							if (strand.size() <= first + offset || first + offset < 0) {
								continue;
							}
							strand[first + offset] = fix[i].second;
						}
					}
				}
			}
			if (strand.size() == DATA_SIZE + DATA_PARITY_SIZE) {
				data = Convertor::DnaToBin(strand);
				err = rs.decode(data);
				if (!IsFailed(err)) {
					this->data[dna_num] = data;
					success = true;
					break;
				}
			}
			int pos = 0;
			while (pos < fix_selector.size() && fix_selector[pos]) {
				fix_selector[pos] = false;
				pos++;
			}
			if (pos == fix_selector.size()) {
				break;
			}
			fix_selector[pos] = true;
		}
		if (success) {
			continue;
		}
		main_dna = data_strand.front();
		data = Convertor::DnaToBin(main_dna);
		this->data[dna_num] = data;
		this->err_cnt++;
	}
	return 0;
}

DNA_Pool::DNA_Pool() {
	this->size = -1;
}

int DNA_Pool::AppendDNA(DNA strand) {
	// Checking Index
	DNA index_seq(strand.begin() + RE_SIZE, strand.begin() + RE_SIZE + INDEX_SIZE + INDEX_OVERFLOW_PARM_SIZE + INDEX_PARITY_SIZE);
	string index_data = Convertor::DnaToBin(index_seq);
	IndexRS index_rs;
	int err = index_rs.decode(index_data);
	if (IsFailed(err)) {
		this->unknown_sequence.push_back(strand);
		return -1;
	}
	int index = ((index_data[0]) << (MAX_INDEX_ORDER - 8)) + (((index_data[1]) >> 8 - (MAX_INDEX_ORDER - 8)) & 3);
	vector<pair<int, int>> overflow_parm_stack;
	for (int i = (8 - (MAX_INDEX_ORDER - 8)) / 2 - 1; i >= 0; i--) {
		int push = ((index_data[1] & 3) + 4 - i) & 3;
		bool pass = false;
		for (int j = 0; j < overflow_parm_stack.size(); j++) {
			if (overflow_parm_stack[j].second == push) {
				overflow_parm_stack[j].first--;
				pass = true;
				break;
			}
		}
		if(!pass) overflow_parm_stack.push_back(make_pair(0, push));
		index_data[1] >>= 2;
	}
	std::sort(overflow_parm_stack.begin(), overflow_parm_stack.end());
	int overflow_parm = overflow_parm_stack.front().second;

	if (overflow_parm) {
		this->size = index + REPETITION - overflow_parm;
	}
	if (this->dna_sequence.size() <= index) {
		this->dna_sequence.resize(index + 1);
	}
	this->dna_sequence[index].push_back(strand);
	return err;
}

int DNA_Pool::Processing() {
	if (IsFailed(this->size)) {
		this->size = this->dna_sequence.size() + REPETITION;
	}
	this->err_cnt = 0;
	this->fatal_err_cnt = 0;
	cout << "\t\tMerging DNA Strands . . ." << endl;
	this->MergeMultipleDNA();
	cout << "\t\tExtracting Datas . . ." << endl;
	this->ExtractDataFromDNA();
	return 0;
}

std::string DNA_Pool::GetData() {
	string ret;
	for (int i = 0; i < this->data.size(); i++) {
		if (this->data[i].size() < BPSToByte(DATA_SIZE)) {
			ret += this->data[i];
			continue;
		}
		for (int j = 0; j < BPSToByte(DATA_SIZE); j++) {
			ret += this->data[i][j];
		}
	}
	return ret;
}

int DNA_Pool::GetSize() {
	return this->size;
}

int DNA_Pool::GetErrCnt() {
	return err_cnt;
}

int DNA_Pool::GetFatalErrCnt() {
	return this->fatal_err_cnt;
}
