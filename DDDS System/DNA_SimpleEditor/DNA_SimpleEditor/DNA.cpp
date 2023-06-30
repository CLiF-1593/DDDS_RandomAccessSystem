#include "DNA.h"
#include <assert.h>
#include <algorithm>
using namespace std;

int GCD(int a, int b) {
	if (a < b) {
		int tmp = a;
		a = b;
		b = tmp;
	}
	while (b) {
		int tmp = a % b;
		a = b;
		b = tmp;
	}
	return a;
}

// =============================================
// ==================== DNA ====================
// =============================================

DNA DNA::slice(int begin, int end) {
	DNA new_dna;
	if(this->empty()) return new_dna;
	for (int i = begin; i < end; i++) {
		int index = i;
		while (index < 0) index += this->size();
		while (index >= (int)this->size()) {
			index -= this->size();
		}
		new_dna.push_back((*this)[index]);
	}
	return new_dna;
}

void DNA::ins(int index, Nucleotide nucleotide) {
	if(0 <= index && index <= this->size()) {
		this->push_back(X);
		for(int i = this->size() - 1; i > index; i--) {
			(*this)[i] = (*this)[i - 1];
		}
		(*this)[index] = nucleotide;
	}
}

Nucleotide DNA::del(int index) {
	if(0 <= index && index < this->size()) {
		Nucleotide nucleotide = (*this)[index];
		for(int i = index; i < this->size() - 1; i++) {
			(*this)[i] = (*this)[i + 1];
		}
		this->pop_back();
		return nucleotide;
	}
	return X;
}

std::string DNA::to_binary() {
	std::string ret;
	while (this->size() & 3) {
		this->push_back(A);
	}
	for (int i = 0; i < this->size(); i += 4) {
		char c = 0;
		for (int j = 0; j < 4; j++) {
			c <<= 2;
			c += (*this)[i + j];
		}
		ret += c;
	}
	return ret;
}

unsigned int DNA::to_int() {
	unsigned int ret = 0;
	for (int i = 0; i < this->size(); i++) {
		ret <<= 2;
		ret += (*this)[i];
	}
	return ret;
}

void DNA::from_binary(std::string binary) {
	DNA ret;
	for (int i = 0; i < binary.size(); i++) {
		DNA section;
		char c = binary[i];
		for (int k = 0; k < 4; k++) {
			section.push_back(static_cast<Nucleotide>(c & 3));
			c >>= 2;
		}
		reverse(section.begin(), section.end());
		ret.insert(ret.end(), section.begin(), section.end());
	}
	(*this) = ret;
	return;
}

void DNA::from_int(int integer, int size) {
	DNA ret;
	for (int i = 0; i < size; i++) {
		ret.push_back(static_cast<Nucleotide>(integer & 3));
		integer >>= 2;
	}
	reverse(ret.begin(), ret.end());
	(*this) = ret;
	return;
}

std::string DNA::str() {
	string ret;
	for (int i = 0; i < this->size(); i++) {
		switch ((*this)[i]) {
			case A: ret += 'A'; break;
			case G: ret += 'G'; break;
			case C: ret += 'C'; break;
			case T: ret += 'T'; break;
			case X: ret += '_'; break;
		}
	}
	return ret;
}

// ===================================================
// ================== Edit Distance ==================
// ===================================================

class ReturnType {
public:
	int distance = -1;
	DNA dna1;
	DNA dna2;

	bool operator<(ReturnType& other) const {
		return this->distance < other.distance;
	}
};

vector<ReturnType> cache;

ReturnType EditDistance(DNA& dna1, DNA& dna2, int index1, int index2) {
	ReturnType ret;
	if (cache[(dna2.size() + 1) * index1 + index2].distance != -1) {
		return cache[(dna2.size() + 1) * index1 + index2];
	}
	if (index1 + index2 == 0) {
		ret.distance = 0;
		cache[(dna2.size() + 1) * index1 + index2] = ret;
		return ret;
	}
	if (index1 == 0) {
		ret.distance = index2;
		for (int i = 0; i < index2; i++) ret.dna1.push_back(X);
		ret.dna2.insert(ret.dna2.end(), dna2.begin(), dna2.begin() + index2);
		cache[(dna2.size() + 1) * index1 + index2] = ret;
		return ret;
	}
	if (index2 == 0) {
		ret.distance = index1;
		ret.dna1.insert(ret.dna1.end(), dna1.begin(), dna1.begin() + index1);
		for (int i = 0; i < index1; i++) ret.dna2.push_back(X);
		cache[(dna2.size() + 1) * index1 + index2] = ret;
		return ret;
	}
	vector<ReturnType> container;
	container.push_back(EditDistance(dna1, dna2, index1 - 1, index2 - 1));
	container.push_back(EditDistance(dna1, dna2, index1, index2 - 1));
	container.push_back(EditDistance(dna1, dna2, index1 - 1, index2));
	container[0].distance += (dna1[index1 - 1] != dna2[index2 - 1]);
	container[0].dna1.push_back(dna1[index1 - 1]);
	container[0].dna2.push_back(dna2[index2 - 1]);
	container[1].distance++;
	container[1].dna2.push_back(dna2[index2 - 1]);
	container[1].dna1.push_back(X);
	container[2].distance++;
	container[2].dna1.push_back(dna1[index1 - 1]);
	container[2].dna2.push_back(X);
	sort(container.begin(), container.end());
	cache[(dna2.size() + 1) * index1 + index2] = container.front();
	return container.front();
}

int GetDelta(DNA& dna1, DNA& dna2) {
	cache.clear();
	cache.resize((dna1.size() + 1) * (dna2.size() + 1));
	ReturnType ret = EditDistance(dna1, dna2, dna1.size(), dna2.size());
	dna1 = ret.dna1;
	dna2 = ret.dna2;
	return ret.distance;
}

void GetEditDistance(vector<DNA>* dna_set) {
	if (!dna_set || dna_set->empty()) return;

	for (int i = 0; i < dna_set->size() - 1; i++) {
		int index = i;
		int prev_size = 0;
		do {
			GetDelta(dna_set->at(index), dna_set->at(index + 1));
			/*for (int j = 0; j < dna_set->size(); j++) {
				cout << dna_set->at(j).str() << endl;
			}
			cout << endl;*/
		} while (prev_size != dna_set->at(index).size() && index--);
	}

	int max_size = 0;
	for (int i = 0; i < dna_set->size(); i++) {
		if (dna_set->at(i).size() > max_size)
			max_size = dna_set->at(i).size();
	}
	vector<int> x_num;
	for (int i = 0; i < dna_set->size(); i++) {
		while (dna_set->at(i).size() < max_size) {
			dna_set->at(i).push_back(X);
		}
		x_num.push_back(0);
		for (int j = (int)dna_set->at(i).size() - 1; j >= 0; j--) {
			if (dna_set->at(i)[j] == X) x_num.back()++;
			else break;
		}
	}
	sort(x_num.begin(), x_num.end());
	for (int i = 0; i < dna_set->size(); i++) {
		for (int j = 0; j < x_num.front(); j++) {
			dna_set->at(i).pop_back();
		}
	}
}

void GetCandidateList(DNA & base_dna, vector<InsCandidate>& ins_candidate, vector<DNA>* dna_set) {
	if (!dna_set || dna_set->empty()) return;
	base_dna.clear();
	ins_candidate.clear();
	int dna_max_length = dna_set->front().size();
	int nucleotide_case_number = dna_set->size();
	for (int i = 0; i < dna_max_length; i++) {
		int a = 0, g = 0, c = 0, t = 0;
		for (int j = 0; j < nucleotide_case_number; j++) {
			switch (dna_set->at(j)[i]) {
			case A: a++; break;
			case G: g++; break;
			case C: c++; break;
			case T: t++; break;
			}
		}
		if (a == nucleotide_case_number) base_dna.push_back(A);
		else if (g == nucleotide_case_number) base_dna.push_back(G);
		else if (c == nucleotide_case_number) base_dna.push_back(C);
		else if (t == nucleotide_case_number) base_dna.push_back(T);
		else {
			base_dna.push_back(X);
			if (a) ins_candidate.push_back(InsCandidate(a, i, A));
			if (g) ins_candidate.push_back(InsCandidate(g, i, G));
			if (c) ins_candidate.push_back(InsCandidate(c, i, C));
			if (t) ins_candidate.push_back(InsCandidate(t, i, T));
		}
	}
	assert(base_dna.size() == dna_max_length);
	sort(ins_candidate.begin(), ins_candidate.end(), greater<InsCandidate>());
	for (int i = 0; i < ins_candidate.size(); i++) {
		if (ins_candidate[i].frequency < (double)nucleotide_case_number * CANDIDATE_RATIO) {
			ins_candidate.erase(ins_candidate.begin() + i, ins_candidate.end());
			break;
		}
	}
}

DNA Merge(int fix_number, DNA& base_dna, vector<InsCandidate>& ins_candidate) {
	int cnt = fix_number;
	DNA potent_dna = base_dna;
	int candidate_index = 0;
	while (cnt) {
		if (candidate_index == ins_candidate.size()) break;
		int pos = ins_candidate[candidate_index].index;
		if (potent_dna[pos] == X) {
			potent_dna[pos] = ins_candidate[candidate_index].nucleotide;
			cnt--;
		}
		candidate_index++;
	}
	for (int i = 0; i < potent_dna.size(); i++) {
		if (potent_dna[i] == X) {
			potent_dna.del(i);
			i--;
		}
	}
	return potent_dna;
}

// ==================================================
// ================== DNA_Analyzer ==================
// ==================================================


void DNA_Analyzer::SetEditDistance() {
	GetEditDistance(this->dna_set);
}

void DNA_Analyzer::SetCandidate() {
	GetCandidateList(this->base_dna, this->ins_candidate, this->dna_set);
}

void DNA_Analyzer::SetCorrectDNA() {
	if (!this->dna_set || this->dna_set->empty()) return;

	// No Error Detected
	if (this->ins_candidate.size() == 0) {
		DataRS rs;
		string str = this->base_dna.to_binary();
		if (str.empty()) return;
		int err = rs.decode(str);
		if (!IsFailed(err)) {
			for (int i = 0; i < BPSToByte(DATA_PARITY_SIZE) && !str.empty(); i++) str.pop_back();
			this->result = str;
		}
		return;
	}
	
	// Get Fix Number
	int fix_number = DATA_SIZE + DATA_PARITY_SIZE;
	for (int i = 0; i < this->base_dna.size(); i++) {
		if (this->base_dna[i] != X) fix_number--;
	}
	if (fix_number < 0) {
		fix_number *= -1;
		vector<bool> fix_selector;
		fix_selector.resize(this->base_dna.size());
		for (int i = 0; i < fix_selector.size(); i++) fix_selector[i] = false;
		if (!fix_selector.empty()) fix_selector[0] = true;

		int timer = clock();

		while (true) {
			if (clock() - timer > MAXIMUM_CALCULATING_TIME) break;

			DNA base = this->base_dna;
			int cnt = 0;
			bool pass = false;
			for (int i = 0; i < fix_selector.size(); i++) {
				if (fix_selector[i]) {
					if (base[i] == X) {
						pass = true;
						goto ESCAPSE_DELETITION;
					}
					else {
						base[i] = X;
						cnt++;
					}
				}
			}
			if (cnt != fix_number) pass = true;
		ESCAPSE_DELETITION:

			if (!pass) {
				DNA dna;
				for (int i = 0; i < base.size(); i++) {
					if (base[i] != X) dna.push_back(base[i]);
				}
				DataRS rs;
				string str = dna.to_binary();
				int err = rs.decode(str);
				if (!IsFailed(err)) {
					for (int i = 0; i < BPSToByte(DATA_PARITY_SIZE) && !str.empty(); i++) str.pop_back();
					this->result = str;
					return;
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
		return;
	}

	// Step 1 : Fix DNA based on base DNA and potent insertion candidates
	DNA potent_dna = Merge(fix_number, this->base_dna, this->ins_candidate);
	/*cout << endl << potent_dna.str() << endl;
	for (int i = 0; i < this->dna_set->size(); i++) {
		cout << this->dna_set->at(i).str() << endl;
	}*/
	string str = potent_dna.to_binary();
	DataRS rs;
	int err = rs.decode(str);
	if (!IsFailed(err)) {
		for (int i = 0; i < BPSToByte(DATA_PARITY_SIZE) && !str.empty(); i++) str.pop_back();
		this->result = str;
		return;
	}

	// Step 2 : Fix DNA based on base DNA and all insertion candidates
	vector<bool> fix_selector;
	fix_selector.resize(this->ins_candidate.size());
	for (int i = 0; i < fix_selector.size(); i++) fix_selector[i] = false;
	if(this->ins_candidate.size() > 0) fix_selector[0] = true;

	int timer = clock();

	while (true) {
		if (clock() - timer > MAXIMUM_CALCULATING_TIME) break;

		DNA base = this->base_dna;
		int cnt = 0;
		bool pass = false;
		for (int i = 0; i < fix_selector.size(); i++) {
			if (fix_selector[i]) {
				InsCandidate candidate = this->ins_candidate[i];
				if (base[candidate.index] == X) {
					base[candidate.index] = candidate.nucleotide;
					cnt++;
				}
				else {
					pass = true;
					goto ESCAPSE;
				}
			}
		}
		if (cnt != fix_number) pass = true;
		ESCAPSE:

		if (!pass) {
			DNA dna;
			for (int i = 0; i < base.size(); i++) {
				if (base[i] != X) dna.push_back(base[i]);
			}
			DataRS rs;
			string str = dna.to_binary();
			int err = rs.decode(str);
			if (!IsFailed(err)) {
				for (int i = 0; i < BPSToByte(DATA_PARITY_SIZE) && !str.empty(); i++) str.pop_back();
				this->result = str;
				return;
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
}


InsCandidate::InsCandidate() {
	this->frequency = 0;
	this->index = 0;
	this->nucleotide = X;
}

InsCandidate::InsCandidate(int frequency, int index, Nucleotide nucleotide) {
	this->index = index;
	this->nucleotide = nucleotide;
	this->frequency = frequency;
}

bool InsCandidate::operator<(const InsCandidate& other) const {
	return this->frequency < other.frequency;
}

bool InsCandidate::operator>(const InsCandidate& other) const {
	return this->frequency > other.frequency;
}

void DNA_Analyzer::InitDNA(DNA_Set *dna_set) {
	this->dna_set = dna_set;
	this->result.clear();
	this->missing = 0;
}

void DNA_Analyzer::Analyze() {
	/*for (int i = 0; i < this->dna_set->size(); i++) {
		cout << this->dna_set->at(i).str() << endl;
	}*/
	//cout << "\t\tSet Edit Distance . . ." << endl;
	this->SetEditDistance();
	/*cout << endl;
	for (int i = 0; i < this->dna_set->size(); i++) {
		cout << this->dna_set->at(i).str() << endl;
	}*/
	//cout << "\t\tSet Candidate . . ." << endl;
	this->SetCandidate();
	//cout << "CN : " << this->ins_candidate.size() << " / ";
	//cout << "\t\tCorrecting DNA . . ." << endl;
	/*for (int i = 0; i < this->dna_set->size(); i++) {
		cout << "\t\t\t" << this->dna_set->at(i).str() << endl;
	}*/
	//cout << "\t\tEnd" << endl;
	this->SetCorrectDNA();
}

std::string DNA_Analyzer::GetData() {
	return this->result;
}

std::string DNA_Analyzer::GetLowQualityData() {
	DNA final_dna;
	if (!this->dna_set || this->dna_set->empty()) return "";
	GetEditDistance(this->dna_set);
	for (int i = 0; i < dna_set->front().size(); i++) {
		vector<pair<int, Nucleotide>> stack;
		for (int j = 0; j < dna_set->size(); j++) {
			Nucleotide push = dna_set->at(j)[i];
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
	string str = final_dna.to_binary();
	str.resize(BPSToByte(DATA_SIZE));
	return str;
}
