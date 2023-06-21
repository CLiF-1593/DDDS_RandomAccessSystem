#pragma once
#include "Generator.h"
#include "ScoreNode.h"

class Encoder : public Generator {
private:
	std::vector<std::wstring> file_path;
	std::vector<std::string> data;

public:
	int Input() override;
	int Generate() override;
	int Output() override;
};

