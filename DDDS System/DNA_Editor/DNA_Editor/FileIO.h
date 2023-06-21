#pragma once
#include <iostream>
#include <vector>
#include <string>

namespace FileIO {
	std::vector<std::wstring> GetFileNames();
	std::string ReadFileData(std::string file_path);
	int SaveFileData(std::string file_path, std::string data);
};