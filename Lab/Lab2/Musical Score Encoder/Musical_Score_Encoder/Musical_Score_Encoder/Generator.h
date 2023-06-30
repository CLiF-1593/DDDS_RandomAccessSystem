#pragma once
#include <vector>
#include <string>
class Generator {
public:
	virtual int Input() abstract;
	virtual int Generate() abstract;
	virtual int Output() abstract;
};

