#include "FileIO.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <atlstr.h>
#include <shlobj_core.h>
#include <filesystem>
#include <ProtableFileDialogs.h> //For Open Files with Multiplatforms

using namespace FileIO;
using namespace std;

vector<string> FileIO::GetFileNames() {
	auto selection = pfd::open_file("Open File", "", { "All Files", "*" }, pfd::opt::force_path | pfd::opt::multiselect).result();
	return selection;
}

std::string FileIO::ReadFileData(std::string file_path) {
	FILE* f;
	string data;
	fopen_s(&f, file_path.c_str(), "rb");
	while (feof(f) == 0) {
		data.push_back(fgetc(f));
	}
	fclose(f);
	data.pop_back();
	return data;
}

int FileIO::SaveFileData(std::string file_path, std::string data) {
	FILE* f;
	fopen_s(&f, file_path.c_str(), "wb");
	for (int i = 0; i < data.size(); i++) {
		fputc(data[i], f);
	}
	fclose(f);
    return 0;
}
