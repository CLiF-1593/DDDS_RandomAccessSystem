#pragma once
#include <string>
#include "Config.h"
class Convertor {
public:
	static DNA BinToDna(std::string dat);
	static std::string DnaToBin(DNA dna);
};

