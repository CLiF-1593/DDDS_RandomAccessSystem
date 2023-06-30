#pragma once
#include "TinyXML.h"
#include <string>

class Attribute {
private:
	int division;
	int key_fifths;
	int time_beats;
	int time_beat_type;
	std::string clef_sign;

public:
	Attribute();
	Attribute(tinyxml2::XMLNode* xml_node);

	tinyxml2::XMLNode* SetXML(tinyxml2::XMLDocument** doc);

	Attribute* SetDivision(int division);
	Attribute* SetKey(int key);
	Attribute* SetTimeBeats(int beats);
	Attribute* SetTimeBeatType(int beat_type);
	Attribute* SetClefSign(std::string clef);

	int GetDivision();
	int GetKey();
	int GetTimeBeats();
	int GetTimeBeatType();
	std::string GetClefSign();
};

/*
* Note Format
* Pitch = Octave * 12 + Step + Alter
* Duration = 2^N / 64
* 
* {[duration(3)][dot(1)][tie(1)][triplet(1)][semitone/star(2)]}{[chord(1)][pitch(7)]} //2 byte
*/

class Note {
private:
	int octave;
	char step;
	int alter;
	double duration;
	bool dot;
	bool rest;
	bool chord;
	bool tie;
	bool triplet;
	bool star;

public:
	Note();
	Note(int note_byte);
	Note(tinyxml2::XMLNode* xml_node, Attribute attribute);

	void Set(int note_byte);
	void Set(tinyxml2::XMLNode* xml_node, Attribute attribute);
	int GetNoteByte();
	tinyxml2::XMLNode* GetXML(Attribute attribute, tinyxml2::XMLDocument** doc, bool pitched, int voice_num);

	Note* SetOctave(int value);
	Note* SetStep(char value);
	Note* SetAlter(int value);
	Note* SetDuration(double value);
	Note* SetDot(bool value);
	Note* SetRest(bool value);
	Note* SetChord(bool value);
	Note* SetTie(bool value);
	Note* SetTriplet(bool value);
	Note* SetStar(bool value);

	int GetOctave();
	char GetStep();
	int GetAlter();
	double GetDuration();
	bool GetDot();
	bool GetRest();
	bool GetChord();
	bool GetTie();
	bool GetTriplet();
	bool GetStar();

	bool IsBackup();

	std::string GetInstrument();

	std::string string();
};

void InitTie();

tinyxml2::XMLNode* GetMeasureXML(tinyxml2::XMLDocument** doc, int measure_num);
tinyxml2::XMLNode* GetBackupXML(tinyxml2::XMLDocument** doc, int duration);