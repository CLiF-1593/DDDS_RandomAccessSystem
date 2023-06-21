#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "TinyXML.h"
enum FileType { MusicXML, TEMPLATE, MusicDNA };

namespace FileIO {
	std::vector<std::wstring> GetFileNames(FileType type);
	std::string ReadFileData(std::string file_path);
	int SaveFileData(std::string file_path, std::string data);
};

namespace FileXML {
	tinyxml2::XMLDocument* Read(std::wstring path);
	std::vector<tinyxml2::XMLNode*> GetNoteXML(tinyxml2::XMLDocument** root);
	tinyxml2::XMLNode* GetAttribute(tinyxml2::XMLDocument** root);
	void Write();
}