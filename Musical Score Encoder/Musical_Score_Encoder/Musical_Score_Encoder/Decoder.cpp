#include "Decoder.h"
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

int Decoder::Input() {
	cout << "> Select the Files (MusicDNA)" << endl;
	this->file_path = FileIO::GetFileNames(MusicDNA);
    return 0;
}

int Decoder::Generate() {
	this->xml_file_datas.clear();
	for (int i = 0; i < this->file_path.size(); i++) {
		string str;
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		str = converter.to_bytes(this->file_path[i]);
		cout << "\tPath : " << str << endl;
		string data = FileIO::ReadFileData(str);
		vector<Note> note_list;
		cout << "\tReading . . ." << endl;
		for (int j = 0; j < data.size(); j += 2) {
			int b1 = (unsigned char)data[j];
			int b2 = (unsigned char)data[j + 1];
			int byte = (b1 << 8) + b2;
			note_list.push_back(Note(byte));
			//cout << "\t\t" << note_list.back().string() << endl;
		}

		//int key, beats, beat_type;
		//std::string clef_sign;
		int multi_note, pitched;
		/*cout << "\t> Key : ";
		cin >> key;
		cout << "\t> Beats : ";
		cin >> beats;
		cout << "\t> Beat Type : ";
		cin >> beat_type;
		cout << "\t> Clef Sign : ";
		cin >> clef_sign;*/
		cout << "\t> Multinote : ";
		cin >> multi_note;

		cout << "\t> Pitched : ";
		cin >> pitched;

		cout << "\t> Select the Template File" << endl;
		auto templates = FileIO::GetFileNames(TEMPLATE);
		if (templates.empty()) {
			continue;
		}
		auto template_path = templates[0];

		cout << "\tParsing Template . . ." << endl;
		auto doc_template = FileXML::Read(template_path);

		
		cout << "\tInserting Attribute to Music XML . . ." << endl;
		XMLNode* root = doc_template;
		XMLNode* part = root->FirstChildElement("score-partwise")->FirstChildElement("part");
		int measure_cnt = 1;
		auto measure_xml = GetMeasureXML(&doc_template, measure_cnt++);
		
		Attribute attribute(FileXML::GetAttribute(&doc_template));
		//attribute.SetKey(key)->SetTimeBeats(beats)->SetTimeBeatType(beat_type)->SetClefSign(clef_sign);
		//attribute.SetXML(&doc_template);

		cout << "\tInserting Notes to Music XML . . ." << endl;
		InitTie();

		double measure = (double)attribute.GetTimeBeats() / (double)attribute.GetTimeBeatType();
		if (multi_note) measure *= 2.0;

		double beat_cnt = 0;
		int voice_num = 1;
		bool backuped = false;
		for (int i = 0; i < note_list.size(); i++) {
			auto xml = note_list[i].GetXML(attribute, &doc_template, pitched, voice_num);
			measure_xml->InsertEndChild(xml);
			if (!note_list[i].GetTriplet() && !note_list[i].GetChord()) {
				beat_cnt += note_list[i].GetDuration() * (note_list[i].GetDot() ? 1.5 : 1.0);
			}
			if (i != note_list.size() - 1 && !note_list[i + 1].GetChord()) {
				if (beat_cnt >= measure - 0.0001) {
					beat_cnt -= measure;
					part->InsertEndChild(measure_xml);
					measure_xml = GetMeasureXML(&doc_template, measure_cnt++);
					backuped = false;
					voice_num = 1;
				}
				if (multi_note && !backuped && beat_cnt >= measure / 2.0 - 0.0001) {
					auto backup_xml = GetBackupXML(&doc_template, attribute.GetDivision() * measure * 2.0);
					measure_xml->InsertEndChild(backup_xml);
					backuped = true;
					voice_num++;
				}
			}
		}
		if (measure_xml->FirstChild()) {
			part->InsertEndChild(measure_xml);
		}

		cout << "\tConverting Music XML to String . . ." << endl;
		XMLPrinter printer;
		doc_template->Accept(&printer);

		string xml_string = printer.CStr();
		this->xml_file_datas.push_back(xml_string);
		cout << endl;
	}
	return 0;
}

int Decoder::Output() {
	cout << "Saving Music DNA Files . . ." << endl;
	for (int i = 0; i < this->file_path.size(); i++) {
		cout << "\tPath : ";
		wcout << this->file_path[i] + L".musicxml" << endl;
		string str;
		wstring_convert<codecvt_utf8<wchar_t>> converter;
		str = converter.to_bytes(this->file_path[i] + L".musicxml");
		FileIO::SaveFileData(str, this->xml_file_datas[i]);
	}
	return 0;
}
