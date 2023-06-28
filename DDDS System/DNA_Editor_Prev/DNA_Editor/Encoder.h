#pragma once
#include "Generator.h"
#include <vector>
#include "DNA.h"

class Encoder : public Generator {
private:
	std::vector<std::vector<DNA>> dna_set;
	std::vector<std::wstring> path;
public:
	int Input() override;
	int Generate(std::vector<std::wstring> &path) override;
	int Output() override;
};