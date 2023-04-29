#pragma once
#include "Generator.h"
class Decoder : public Generator{
public:
	int Input() override;
	int Generate(std::vector<std::wstring> *path) override;
};

