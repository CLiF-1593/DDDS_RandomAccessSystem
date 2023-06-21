#include "DeltaDNA.h"
#include <algorithm>
#include "DNA.h"
#include <iostream>
using namespace std;

class ReturnType{
public:
	int distance = -1;
	DNA dna1;
	DNA dna2;

	bool operator <(ReturnType& other) {
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
	container.push_back(EditDistance(dna1, dna2, index1 - 1, index2));
	container.push_back(EditDistance(dna1, dna2, index1, index2 - 1));
	container.push_back(EditDistance(dna1, dna2, index1 - 1, index2 - 1));
	container[0].distance++;
	container[0].dna1.push_back(dna1[index1 - 1]);
	container[0].dna2.push_back(X);
	container[1].distance++;
	container[1].dna2.push_back(dna2[index2 - 1]);
	container[1].dna1.push_back(X);
	container[2].distance += (dna1[index1 - 1] != dna2[index2 - 1]);
	container[2].dna1.push_back(dna1[index1 - 1]);
	container[2].dna2.push_back(dna2[index2 - 1]);
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
