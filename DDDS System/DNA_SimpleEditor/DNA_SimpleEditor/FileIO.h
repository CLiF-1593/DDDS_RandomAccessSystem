#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace FileIO {
	std::vector<std::string> GetFileNames();
	std::string ReadFileData(std::string file_path);
	int SaveFileData(std::string file_path, std::string data);
};