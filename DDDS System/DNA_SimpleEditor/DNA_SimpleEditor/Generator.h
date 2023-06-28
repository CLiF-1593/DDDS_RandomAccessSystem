#pragma once
#include <vector>
#include <string>
class Generator {
public:
	virtual int Input() abstract;
	virtual int Generate(std::vector<std::string> &path) abstract;
	virtual int Output() abstract;
};

