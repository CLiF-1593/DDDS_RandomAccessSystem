#pragma once
#include "Generator.h"
#include "DNA.h"
class Decoder : public Generator{
public:
	int Input() override;
	int Generate(std::vector<std::string> &path) override;
	int Output() override;

private:
	std::vector<std::string> path;
	std::vector<std::string> result;
};

