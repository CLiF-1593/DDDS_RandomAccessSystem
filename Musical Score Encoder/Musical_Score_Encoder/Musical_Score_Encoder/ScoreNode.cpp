#include "ScoreNode.h"
#include <string>
#include <iostream>
#include "FileIO.h"
using namespace std;
using namespace tinyxml2;

#define assign(x, y) Assign(x, to_string(y))

bool tie_start = true;

string Assign(tinyxml2::XMLNode* xml_node, string origin) {
	if (xml_node) {
		return xml_node->ToElement()->GetText();
	}
	return origin;
}

Note::Note() {
	this->alter = 0;
	this->chord = false;
	this->dot = false;
	this->duration = 0;
	this->octave = 0;
	this->rest = false;
	this->star = false;
	this->step = 0;
	this->tie = false;
	this->triplet = false;
}

Note::Note(int note_byte) {
	this->Set(note_byte);
}

Note::Note(tinyxml2::XMLNode* xml_node, Attribute attribute) {
	this->Set(xml_node, attribute);
}

void Note::Set(int note_byte) {
	int beat = (note_byte >> 8) & 255;
	int pitch = (note_byte) & 255;

	this->duration = pow(2.0, (double)((beat & 0b11100000) >> 5)) / 64.0;
	this->dot = beat & 0b00010000;
	this->tie = beat & 0b00001000;
	this->triplet = beat & 0b00000100;
	int semitone_and_star = beat & 0b00000011;
	this->star = false;

	this->chord = pitch & 0b10000000;
	pitch = pitch & 0b01111111;
	if (pitch > 88) {
		this->rest = true;
		this->octave = 0;
		this->step = 0;
		this->alter = 0;
	}
	else {
		int pseudo_step = pitch % 12;
		this->octave = pitch / 12;
		// A, A#, B, C, C#, D, D#, E, F, F#, G, G#
		char step_table[] = { 'A', 'A', 'B', 'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G' };
		char semitone[] = { 0 ,  1 ,  0 ,  0 ,  1 ,  0 ,  1 ,  0 ,  0 ,  1 ,  0 ,  1 };
		this->step = step_table[pseudo_step];
		if (semitone[pseudo_step] || semitone_and_star >= 2) {
			switch (semitone_and_star) {
			case 0: this->alter = 1; break;		// sharp
			case 1: this->alter = -1; break;	// flat
			case 2: this->alter = 2; break;		//double sharp
			case 3: this->alter = -2; break;	// double falt
			}
		}
		else {
			this->star = semitone_and_star;
			this->alter = 0;
		}
		this->rest = false;
	}
}

void Note::Set(tinyxml2::XMLNode* xml_node, Attribute attribute) {
	*this = Note();
	double div = attribute.GetDivision();
	this->rest = (bool)xml_node->FirstChildElement("rest");
	if (!this->rest) {
		if (xml_node->FirstChildElement("pitch")) {
			this->octave = stoi(assign(xml_node->FirstChildElement("pitch")->FirstChildElement("octave"), this->octave));
			this->step = assign(xml_node->FirstChildElement("pitch")->FirstChildElement("step"), this->step)[0];
			this->alter = stoi(assign(xml_node->FirstChildElement("pitch")->FirstChildElement("alter"), this->alter));
		}
		else {
			this->octave = stoi(assign(xml_node->FirstChildElement("unpitched")->FirstChildElement("display-octave"), this->octave));
			this->step = assign(xml_node->FirstChildElement("unpitched")->FirstChildElement("display-step"), this->step)[0];
		}
	}
	this->duration = stoi(assign(xml_node->FirstChildElement("duration"), this->duration));
	this->duration = this->duration / div / 4.0;
	this->dot = (bool)xml_node->FirstChildElement("dot");
	if (this->dot) {
		this->duration * 2.0 / 3.0;
	}
	this->chord = (bool)xml_node->FirstChildElement("chord");
	this->tie = (bool)xml_node->FirstChildElement("tie");
	if (this->tie && xml_node->FirstChildElement("tie")->NextSiblingElement("tie")) {
		this->tie = false;
	}
	this->star = (bool)xml_node->FirstChildElement("notehead");
	this->triplet = false;
	if (xml_node->FirstChildElement("time-modification")) {
		if (Assign(xml_node->FirstChildElement("time-modification")->FirstChildElement("actual-notes"), "0") == "3") {
			this->triplet = true;
		}
	}
}

int Note::GetNoteByte() {
	int beat = 0, pitch = 0;
	beat += (int)(log2(this->duration * 64)) << 5;
	beat += (int)(this->dot) << 4;
	beat += (int)(this->tie) << 3;
	beat += (int)(this->triplet) << 2;
	if (this->alter) {
		switch (this->alter) {
		case 1: beat += 0; break;
		case -1: beat += 1; break;
		case 2: beat += 2; break;
		case -2: beat += 3; break;
		}
	}
	else {
		beat += this->star;
	}

	pitch += (int)(this->chord) << 7;
	if (this->rest) {
		pitch = 114;
	}
	else {
		char step_table[] = { 'A', 'A', 'B', 'C', 'C', 'D', 'D', 'E', 'F', 'F', 'G', 'G' };
		pitch += this->octave * 12 + this->alter;
		for (int i = 0; i < 12; i++) {
			if (step_table[i] == this->step) {
				pitch += i;
				break;
			}
		}
	}
	return (beat << 8) + pitch;
}

tinyxml2::XMLNode* Note::GetXML(Attribute attribute, tinyxml2::XMLDocument** doc, bool pitched, int voice_num) {
	XMLElement* note_element = (*doc)->NewElement("note");

	if (this->rest) {
		XMLElement* rest_element = (*doc)->NewElement("rest");
		note_element->InsertEndChild(rest_element);
	}
	else {
		XMLElement* pitch_element;
		XMLElement* pitch_step_element;
		XMLElement* pitch_octave_element;
		if (pitched) {
			pitch_element = (*doc)->NewElement("pitch");
			pitch_step_element = (*doc)->NewElement("step");
			pitch_octave_element = (*doc)->NewElement("octave");
		}
		else {
			pitch_element = (*doc)->NewElement("unpitched");
			pitch_step_element = (*doc)->NewElement("display-step");
			pitch_octave_element = (*doc)->NewElement("display-octave");
		}
		std::string str;
		str.push_back(this->step);
		pitch_step_element->SetText(str.c_str());
		pitch_octave_element->SetText(this->octave);
		pitch_element->InsertEndChild(pitch_step_element);
		pitch_element->InsertEndChild(pitch_octave_element);
		if (this->alter && pitched) {
			XMLElement* pitch_alter_element = (*doc)->NewElement("alter");
			pitch_alter_element->SetText(this->alter);
			pitch_element->InsertEndChild(pitch_alter_element);
		}
		note_element->InsertEndChild(pitch_element);

		if (!pitched) {
			XMLElement* element = (*doc)->NewElement("instrument");
			element->SetAttribute("id", this->GetInstrument().c_str());
			note_element->InsertEndChild(element);
		}
	}
	
	XMLElement* duration_element = (*doc)->NewElement("duration");
	duration_element->SetText((int)(this->duration * (double)attribute.GetDivision() * 4.0 * (this->dot ? 1.5 : 1.0)));
	note_element->InsertEndChild(duration_element);

	std::string duration_type[] = { "whole", "half", "quarter", "eighth", "16th", "32th", "64th" };
	int index = 6 - log2(this->duration * 64.0);
	XMLElement* type_element = (*doc)->NewElement("type");
	type_element->SetText(duration_type[index].c_str());
	note_element->InsertEndChild(type_element);

	XMLElement* voice_element = (*doc)->NewElement("voice");
	voice_element->SetText(voice_num);
	note_element->InsertEndChild(voice_element);

	if (this->chord) {
		XMLElement* element = (*doc)->NewElement("chord");
		note_element->InsertEndChild(element);
	}
	if (this->dot) {
		XMLElement* element = (*doc)->NewElement("dot");
		note_element->InsertEndChild(element);
	}
	if (this->tie) {
		XMLElement* element = (*doc)->NewElement("tie");
		if ((tie_start && !this->chord) || (!tie_start && this->chord)) {
			element->SetAttribute("type", "start");
			if(!this->chord) tie_start = false;
		}
		else {
			element->SetAttribute("type", "stop");
			if (!this->chord) tie_start = true;
		}
		note_element->InsertEndChild(element);
	}
	else if (!tie_start) {
		XMLElement* element = (*doc)->NewElement("tie");
		element->SetAttribute("type", "stop");
		note_element->InsertEndChild(element);
		element = (*doc)->NewElement("tie");
		element->SetAttribute("type", "start");
		note_element->InsertEndChild(element);
	}
	if (this->star) {
		XMLElement* element = (*doc)->NewElement("notehead");
		element->SetText("x");
		note_element->InsertEndChild(element);
	}
	if (this->triplet) {
		XMLElement* element = (*doc)->NewElement("time-modification");
		XMLElement* actual_element = (*doc)->NewElement("actual-notes");
		actual_element->SetText(3);
		XMLElement* normal_element = (*doc)->NewElement("normal-notes");
		normal_element->SetText(2);
		element->InsertEndChild(actual_element);
		element->InsertEndChild(normal_element);
		note_element->InsertEndChild(element);
	}
	return note_element;
}

Note* Note::SetOctave(int value) {
	this->octave = value;
	return this;
}

Note* Note::SetStep(char value) {
	this->step = value;
	return this;
}

Note* Note::SetAlter(int value) {
	this->alter = value;
	return this;
}

Note* Note::SetDuration(double value) {
	this->duration = value;
	return this;
}

Note* Note::SetDot(bool value) {
	this->dot = value;
	return this;
}

Note* Note::SetRest(bool value) {
	this->rest = value;
	return this;
}

Note* Note::SetChord(bool value) {
	this->chord = value;
	return this;
}

Note* Note::SetTie(bool value) {
	this->tie = value;
	return this;
}

Note* Note::SetTriplet(bool value) {
	this->triplet = value;
	return this;
}

Note* Note::SetStar(bool value) {
	this->star = value;
	return this;
}

int Note::GetOctave() {
	return this->octave;
}

char Note::GetStep() {
	return this->step;
}

int Note::GetAlter() {
	return this->alter;
}

double Note::GetDuration() {
	return this->duration;
}

bool Note::GetDot() {
	return this->dot;
}

bool Note::GetRest() {
	return this->rest;
}

bool Note::GetChord() {
	return this->chord;
}

bool Note::GetTie() {
	return this->tie;
}

bool Note::GetTriplet() {
	return this->triplet;
}

bool Note::GetStar() {
	return this->star;
}

#define INST(x, y, z) if(this->step == x && this->octave == y) return z;

std::string Note::GetInstrument() {
	INST('F', 4, "P1-I37");
	INST('D', 5, "P1-I46");
	INST('A', 4, "P1-I42");
	INST('G', 5, "P1-I43");
	INST('C', 5, "P1-I39");
	INST('F', 5, "P1-I51");
	INST('A', 5, "P1-I50");
	INST('E', 5, "P1-I48");
	return "";
}
//* {[duration(3)][dot(1)][tie(1)][triplet(1)][semitone/star(2)]}{[chord(1)][pitch(7)]} //2 byte

string Note::string() {
	return "Duration : " + to_string(this->duration) + ", Dot : " + to_string(this->dot) + ", Tie : " + to_string(this->tie) + ", Triplet : " + to_string(this->triplet)
		+ ", Semitone : " + to_string(this->alter) + ", Star : " + to_string(this->star) + ", Chord : " + to_string(this->chord) + ", Octave : " + to_string(this->octave)
		+ ", Step : " + this->step;
}

Attribute::Attribute() {
	this->division = 4;
	this->key_fifths = 0;
	this->time_beats = 4;
	this->time_beat_type = 4;
	this->clef_sign = 'G';
}

Attribute::Attribute(tinyxml2::XMLNode* xml_node) {
	this->division = 4;
	this->key_fifths = 0;
	this->time_beats = 4;
	this->time_beat_type = 4;
	this->clef_sign = 'G';

	this->division = stoi(assign(xml_node->FirstChildElement("divisions"), this->division));
	this->key_fifths = stoi(assign(xml_node->FirstChildElement("key")->FirstChildElement("fifths"), this->key_fifths));
	this->time_beats = stoi(assign(xml_node->FirstChildElement("time")->FirstChildElement("beats"), this->time_beats));
	this->time_beat_type = stoi(assign(xml_node->FirstChildElement("time")->FirstChildElement("beat-type"), this->time_beat_type));
	this->clef_sign = Assign(xml_node->FirstChildElement("clef")->FirstChildElement("sign"), this->clef_sign);
}

tinyxml2::XMLNode* Attribute::SetXML(tinyxml2::XMLDocument** doc) {
	XMLElement* attr_element = FileXML::GetAttribute(doc)->ToElement();

	XMLElement* div_element = attr_element->FirstChildElement("divisions");
	div_element->SetText(this->division);

	XMLElement* key_element = attr_element->FirstChildElement("key");
	XMLElement* key_fifths_element = key_element->FirstChildElement("fifths");
	key_fifths_element->SetText(this->key_fifths);

	XMLElement* time_element = attr_element->FirstChildElement("time");
	XMLElement* time_beats_element = time_element->FirstChildElement("beats");
	XMLElement* time_beat_type_element = time_element->FirstChildElement("beat-type");
	time_beats_element->SetText(this->time_beats);
	time_beat_type_element->SetText(this->time_beat_type);

	XMLElement* clef_element = attr_element->FirstChildElement("clef");
	XMLElement* clef_sign_element = clef_element->FirstChildElement("sign");
	clef_sign_element->SetText(this->clef_sign.c_str());

	return attr_element;
}

Attribute* Attribute::SetDivision(int division) {
	this->division = division;
	return this;
}

Attribute* Attribute::SetKey(int key) {
	this->key_fifths = key;
	return this;
}

Attribute* Attribute::SetTimeBeats(int beats) {
	this->time_beats = beats;
	return this;
}

Attribute* Attribute::SetTimeBeatType(int beat_type) {
	this->time_beat_type = beat_type;
	return this;
}

Attribute* Attribute::SetClefSign(std::string clef) {
	this->clef_sign = clef;
	return this;
}

int Attribute::GetDivision() {
	return this->division;
}

int Attribute::GetKey() {
	return this->key_fifths;
}

int Attribute::GetTimeBeats() {
	return this->time_beats;
}

int Attribute::GetTimeBeatType() {
	return this->time_beat_type;
}

std::string Attribute::GetClefSign() {
	return this->clef_sign;
}

void InitTie() {
	tie_start = true;
}

tinyxml2::XMLNode* GetMeasureXML(tinyxml2::XMLDocument** doc, int measure_num) {
	if (measure_num == 1) {
		return (*doc)->FirstChildElement("score-partwise")->FirstChildElement("part")->FirstChildElement("measure");
	}
	XMLElement* measure = (*doc)->NewElement("measure");
	measure->SetAttribute("number", to_string(measure_num).c_str());
	return measure;
}

tinyxml2::XMLNode* GetBackupXML(tinyxml2::XMLDocument** doc, int duration) {
	XMLElement* backup = (*doc)->NewElement("backup");
	XMLElement* dur = (*doc)->NewElement("duration");
	dur->SetText(duration);
	backup->InsertFirstChild(dur);
	return backup;
}
