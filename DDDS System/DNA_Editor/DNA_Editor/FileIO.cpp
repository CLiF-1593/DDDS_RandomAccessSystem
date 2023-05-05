#include "FileIO.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <atlstr.h>
#include <shlobj_core.h>
#include <filesystem>
using namespace FileIO;
using namespace std;

vector<wstring> FileIO::GetFileNames() {
	HDC hdc;
	PAINTSTRUCT ps;
	OPENFILENAME ofn;
	wchar_t lpstrFile[MAX_PATH] = L"";
	memset(&ofn, 0, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.lpstrFile = lpstrFile;
	ofn.hwndOwner = NULL;
	ofn.nMaxFile = 256;
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT;

	vector<wstring> path_set;
	if (GetOpenFileName(&ofn) != 0) {
		wchar_t* str = ofn.lpstrFile;
		std::wstring directory = str;
		str += (directory.length() + 1);
		while (*str) {
			std::wstring filename = str;
			str += (filename.length() + 1);
			path_set.push_back(directory + L"\\" + filename);
		}
	}
	return path_set;
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
