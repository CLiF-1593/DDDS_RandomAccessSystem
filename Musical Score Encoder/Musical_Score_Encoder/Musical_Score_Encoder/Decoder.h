#pragma once
#include "Generator.h"
#include "ScoreNode.h"

class Decoder : public Generator {
private:
	std::vector<std::wstring> file_path;
	std::vector<std::string> xml_file_datas;
public:
	int Input() override;
	int Generate() override;
	int Output() override;
};

