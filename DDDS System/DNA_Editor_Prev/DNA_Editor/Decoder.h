#pragma once
#include "Generator.h"
#include "DNA.h"
class Decoder : public Generator{
public:
	int Input() override;
	int Generate(std::vector<std::wstring> &path) override;
	int Output() override;

private:
	std::vector<std::wstring> path;
	std::vector<DNA_Pool> dna_pool_set;
};

