#include "Encoder.h"
#include "FileIO.h"
#include "ScoreNode.h"
#include <iostream>
#include <locale>
#include <string>
#include <codecvt>
#include <sstream>
#include <bitset>
#include "TinyXML.h"
using namespace std;
using namespace tinyxml2;

int Encoder::Input() {
	cout << "> Select the Files (MusicXML)" << endl;
	this->file_path = FileIO::GetFileNames(MusicXML);
    return 0;
}

int Encoder::Generate() {
	this->data.clear();
	for (int i = 0; i < this->file_path.size(); i++) {
		auto doc = FileXML::Read(this->file_path[i]);
		cout << "\tPath : ";
		wcout << this->file_path[i] << endl;

		cout << "\tParsing . . ." << endl;
		auto attr_xml = FileXML::GetAttribute(&doc);
		auto note_xml = FileXML::GetNoteXML(&doc);

		cout << "\tConverting XML to Binary Data . . ." << endl;
		Attribute attr(attr_xml);
		string out = "";
		for (int j = 0; j < note_xml.size(); j++) {
			int byte = Note(note_xml[j], attr).GetNoteByte();
			out += (char)(byte >> 8);
			out += (char)(byte);
			//cout << "\t\t" << this->note_list.back().string();
			//cout << " | Binary : " << bitset<16>(this->note_list.back().GetNoteByte()) << endl;
		}
		this->data.push_back(out);
		cout << endl;
	}
	return 0;
}

int Encoder::Output() {
	cout << "Saving Music DNA Files . . ." << endl;
	for (int i = 0; i < this->file_path.size(); i++) {
		cout << "\tPath : ";
		wcout << this->file_path[i] + L".musicdna" << endl;
		
		string str;
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		str = converter.to_bytes(this->file_path[i] + L".musicdna");
		FileIO::SaveFileData(str, this->data[i]);
	}
	return 0;
}
