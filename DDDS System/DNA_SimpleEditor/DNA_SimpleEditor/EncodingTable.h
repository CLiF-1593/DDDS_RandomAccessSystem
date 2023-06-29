#include <string>

enum EncodingType {
	ASCII,
	ENGLISH_CUSTOM,
	ALL_LANGUAGE_CUSTOM,
	COORDINATE
};

std::string Encoding(EncodingType encoding_type, std::string dat);
std::string Decoding(EncodingType encoding_type, int table_type, std::string dat);

