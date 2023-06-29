#include "FileIO.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <atlstr.h>
#include <shlobj_core.h>
#include <filesystem>
#include <locale>
#include <string>
#include <codecvt>
#include "TinyXML.h"

using namespace FileIO;
using namespace FileXML;
using namespace std;
using namespace tinyxml2;

vector<wstring> FileIO::GetFileNames(FileType type) {
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
	switch (type) {
	case MusicXML:
		ofn.lpstrFilter = L"MusicXML File\0*.xml;*.musicxml\0";
		break;
	case MusicDNA:
		ofn.lpstrFilter = L"MusicDNA File\0*.musicdna\0";
		break;
	case TEMPLATE:
		ofn.lpstrFilter = L"Template File\0*.template\0";
		break;
	}
	ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR | OFN_ALLOWMULTISELECT;

	vector<wstring> path_set;
	if (GetOpenFileName(&ofn) != 0) {
		wchar_t* str = ofn.lpstrFile;
		std::wstring directory = str;
		str += (directory.length() + 1);
		if (!(*str)) {
			path_set.push_back(directory);
		}
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

tinyxml2::XMLDocument* FileXML::Read(std::wstring path) {
	string str;
	wstring_convert<codecvt_utf8<wchar_t>> converter;
	str = converter.to_bytes(path);

	tinyxml2::XMLDocument *doc = new tinyxml2::XMLDocument;
	doc->LoadFile(str.c_str());
	return doc;
}

std::vector<tinyxml2::XMLNode*> FileXML::GetNoteXML(tinyxml2::XMLDocument** root) {
	vector<XMLNode*> vec;
	XMLNode* node;
	bool multi_score = false;
	node = (*root)->FirstChildElement("score-partwise")->FirstChildElement("part")->FirstChildElement("measure");
	for (XMLNode* iter = node; iter; iter = iter->NextSiblingElement("measure")) {
		for (XMLNode* note = iter->FirstChildElement("note"); note; note = note->NextSiblingElement("note")) {
			vec.push_back(note);
		}
		if (iter->FirstChildElement("backup") == nullptr) {
			vec.push_back(nullptr);
		}
	}
	return vec;
}

tinyxml2::XMLNode* FileXML::GetAttribute(tinyxml2::XMLDocument** root) {
	return (*root)->FirstChildElement("score-partwise")->FirstChildElement("part")->FirstChildElement("measure")->FirstChildElement("attributes");
}