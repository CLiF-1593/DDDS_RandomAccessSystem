#include "EncodingTable.h"
#include <vector>
#include <iostream>
#include <string>
using namespace std;

string english_custom_encoding_table_std[] = {
	"", "!", "\"", "&", "\'", "(", ")", ",", ".", ":", ";", "?", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "Ｖ", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
};

string english_custom_encoding_table_1[] = {
	"", "\'", "?", "\"", ":", ",", ")", "(", ".", "!", "&", ";", "U", "B", "C", "W", "S", "Z", "I", "J", "Ｖ", "P", "H", "E", "Q", "O", "K", "N", "T", "A", "F", "Y", "D", "G", "L", "X", "M", "R", "r", "k", "f", "j", "u", "m", "p", "o", "i", "g", "e", "x", "w", "q", "d", "c", "s", "a", "t", "y", "h", "v", "b", "l", "n", "z"
};

string english_custom_encoding_table_2[] = {
	"", ";", ":", ".", ",", ")", "(", "&", "?", "\"", "!", "\'", "L", "K", "U", "D", "Q", "Z", "N", "H", "I", "G", "O", "X", "B", "Ｖ", "A", "T", "P", "W", "M", "E", "R", "Y", "F", "J", "C", "S", "z", "x", "q", "u", "o", "v", "y", "h", "i", "g", "p", "s", "b", "a", "e", "t", "k", "w", "d", "c", "n", "r", "l", "j", "m", "f"
};

string english_custom_encoding_table_3[] = {
	"", "?", ":", "\"", "!", "(", ")", "&", ".", ",", ";", "\'", "W", "U", "F", "O", "Ｖ", "Y", "G", "N", "J", "A", "H", "K", "I", "L", "C", "S", "M", "B", "E", "Q", "P", "D", "Z", "X", "T", "R", "h", "e", "q", "l", "r", "f", "b", "d", "z", "x", "y", "n", "i", "c", "s", "w", "v", "u", "g", "p", "a", "k", "j", "o", "t", "m"
};

string english_custom_encoding_table_4[] = {
	"", "(", ":", "\"", "!", ",", "&", "?", ".", ";", ")", "\'", "K", "T", "A", "B", "J", "O", "Ｖ", "Y", "S", "C", "M", "L", "N", "W", "D", "Z", "E", "X", "U", "P", "F", "Q", "R", "I", "H", "G", "c", "b", "d", "i", "a", "q", "f", "l", "s", "t", "k", "h", "v", "x", "e", "p", "u", "z", "w", "j", "o", "g", "n", "y", "m", "r"
};

string all_lang_custom_encoding_table_std[] = {
	"", "!", "\"", "&", "\'", "(", ")", ",", ".", ":", ";", "?", "`", "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", "À", "Á", "Â", "Ã", "Ä", "Å", "Æ", "Ç", "È", "É", "Ê", "Ë", "Ì", "Í", "Î", "Ï", "Ð", "Ñ", "Ò", "Ó", "Ô", "Õ", "Ö", "Ø", "Ù", "Ú", "Û", "Ü", "Ý", "Þ", "ß", "à", "á", "â", "ã", "ä", "å", "æ", "ç", "è", "é", "ê", "ë", "ì", "í", "î", "ï", "ð", "ñ", "ò", "ó", "ô", "õ", "ö", "ø", "ù", "ú", "û", "ü", "ý", "þ", "ÿ", "Α", "Β", "Γ", "Δ", "Ε", "Ζ", "Η", "Θ", "Ι", "Κ", "Λ", "Μ", "Ν", "Ξ", "Ο", "Π", "Ρ", "Σ", "Τ", "Υ", "Φ", "Χ", "Ψ", "Ω", "ά", "α", "β", "γ", "δ", "ε", "ζ", "η", "θ", "ι", "κ", "λ", "μ", "ν", "ξ", "ο", "π", "ρ", "ς", "σ", "τ", "υ", "φ", "χ", "ψ", "ω", "ا", "ل", "ع", "ر", "ب", "ي", "ة", "𓂋", "𓏤", "𓈖", "𓆎", "𓅓", "𓏏", "𓊖", "𓀀", "Р", "у", "С", "с", "к", "и", "й", "日", "本", "語", "に", "ほ", "ん", "ご", "中", "文", "ह", "ि", "न", "्", "द", "ी", "বাং", "লা", "লি", "পি", "가", "나", "다", "라", "마", "바", "사", "아", "자", "차", "카", "타", "파", "하", "이", "진", "율", "유", "태", "우", "박", "시", "후", "한", "과", "영", "최", "고", "의", "연", "구", "팀", "는", "요", "은", "에", "서", "로"
};

string all_lang_custom_encoding_table_1[] = {
	"", ".", "`", "\"", "!", ";", ",", ":", "&", ")", "\'", "(", "?", "P", "K", "γ", "가", "E", "I", "â", "Å", "n", "υ", "þ", "b", "q", "L", "È", "f", "κ", "у", "Æ", "σ", "V", "õ", "r", "R", "Ο", "न", "A", "시", "本", "G", "Ρ", "β", "p", "l", "ñ", "ν", "ï", "a", "ς", "s", "X", "Ξ", "라", "T", "o", "후", "Ψ", "Û", "に", "्", "Z", "É", "Χ", "å", "ù", "Ü", "사", "자", "ö", "ζ", "F", "Â", "아", "Í", "ी", "Δ", "N", "τ", "ん", "ह", "Ñ", "우", "진", "요", "y", "ε", "ë", "Ω", "x", "Ô", "O", "μ", "바", "Ä", "ο", "이", "Ò", "χ", "Ú", "ô", "i", "ú", "로", "v", "Σ", "í", "ß", "Κ", "θ", "Μ", "ø", "λ", "с", "율", "ि", "Α", "Р", "к", "하", "日", "ξ", "ψ", "Ð", "ä", "и", "Ì", "ê", "Ç", "𓂋", "द", "α", "ι", "π", "Φ", "𓏤", "à", "é", "Y", "타", "파", "W", "j", "Ν", "ü", "û", "m", "Ö", "서", "𓀀", "Η", "w", "다", "M", "J", "차", "𓆎", "Õ", "𓏏", "𓊖", "𓈖", "은", "𓅓", "Π", "Β", "Ê", "Ε", "에", "Ù", "Þ", "ð", "B", "마", "카", "С", "ó", "Τ", "η", "á", "박", "Ó", "ã", "Ý", "Θ", "g", "Ë", "Γ", "ά", "H", "ò", "ا", "ل", "ع", "ر", "ب", "ي", "ة", "D", "e", "u", "t", "S", "c", "h", "나", "î", "中", "文", "বাং", "লা", "লি", "পি", "Υ", "는", "유", "ý", "k", "ω", "Q", "φ", "Ø", "è", "U", "δ", "ご", "ì", "Ι", "태", "ç", "й", "한", "과", "영", "최", "고", "의", "연", "구", "팀", "z", "Á", "æ", "Î", "ÿ", "À", "語", "d", "Λ", "Ã", "Ζ", "Ï", "C", "ρ", "ほ"
};

std::string Encoding(EncodingType encoding_type, std::string dat) {
	std::string ret;
	if (encoding_type == ASCII) return dat;
	if (encoding_type == ENGLISH_CUSTOM) {
		unsigned char c = 0;
		int cnt = 0;
		for (int i = 0; i < dat.size(); i++) {
			int index = 0;
			for (int k = 0; k < 64; k++) {
				string str;
				str.push_back(dat[i]);
				if (str == english_custom_encoding_table_std[k]) {
					index = k;
					break;
				}
			}
			if (index) {
				switch (cnt % 4) {
				case 0:
					c = index << 2;
					break;
				case 1:
					c += index >> 4;
					ret.push_back(c);
					c = index << 4;
					break;
				case 2:
					c += index >> 2;
					ret.push_back(c);
					c = index << 6;
					break;
				case 3:
					c += index;
					ret.push_back(c);
					c = 0;
					break;
				}
				cnt++;
			}
		}
		if(c) ret.push_back(c);
		return ret;
	}
	if (encoding_type == ALL_LANGUAGE_CUSTOM) {
		string str;
		string ret;
		for (int i = 0; i < dat.size() - 1; i++) {
			if (dat[i] == '\r' && dat[i + 1] == '\n') {
				for (int k = 0; k < 256; k++) {
					if(str == all_lang_custom_encoding_table_std[k]) {
						ret.push_back(k);
						str.clear();
						break;
					}
				}
				i++;
			}
			else {
				str += dat[i];
			}
		}
		for (int k = 0; k < 256; k++) {
			if (str == all_lang_custom_encoding_table_std[k]) {
				ret.push_back(k);
				break;
			}
		}
		return ret;
	}
	if (COORDINATE) {
		unsigned int num = 0;
		string ret;
		for (int i = 0; i < dat.size(); i++) {
			if (dat[i] == '\n') {
				ret.push_back((unsigned char)(num >> 24));
				ret.push_back((unsigned char)(num >> 16));
				ret.push_back((unsigned char)(num >> 8));
				ret.push_back((unsigned char)(num));
				num = 0;
			}
			else {
				unsigned int n = dat[i] - '0';
				if (0 <= n && n <= 9) {
					num *= 10;
					num += n;
				}
			}
		}
		return ret;
	}
}

std::string Decoding(EncodingType encoding_type, int table_type, std::string dat) {
	std::string ret;
	if (encoding_type == ASCII) return dat;
	if (encoding_type == ENGLISH_CUSTOM) {
		vector<int> index;
		unsigned char ind = 0;
		for (int i = 0; i < dat.size(); i++) {
			unsigned char d = dat[i];
			switch (i % 3) {
			case 0:
				ind = d >> 2;
				index.push_back(ind);
				ind = d << 6;
				ind >>= 2;
				break;
			case 1:
				ind += d >> 4;
				index.push_back(ind);
				ind = d << 4;
				ind >>= 2;
				break;
			case 2:
				ind += d >> 6;
				index.push_back(ind);
				ind = d << 2;
				ind >>= 2;
				index.push_back(ind);
				break;
			}
		}
		for (int i = 0; i < index.size(); i++) {
			switch (table_type) {
			case 0 : ret += english_custom_encoding_table_std[index[i]]; break;
			case 1 : ret += english_custom_encoding_table_1[index[i]]; break;
			case 2 : ret += english_custom_encoding_table_2[index[i]]; break;
			case 3 : ret += english_custom_encoding_table_3[index[i]]; break;
			case 4 : ret += english_custom_encoding_table_4[index[i]]; break;
			}
		}
		return ret;
	}
	if (encoding_type == ALL_LANGUAGE_CUSTOM) {
		string ret;
		for (int i = 0; i < dat.size(); i++) {
			switch (table_type) {
			case 0: ret += (all_lang_custom_encoding_table_std[(unsigned char)dat[i]]); break;
			case 1: ret += (all_lang_custom_encoding_table_1[(unsigned char)dat[i]]); break;
			}
		}
		return ret;
	}
	if (COORDINATE) {
		unsigned int num = 0;
		string ret;
		for (int i = 0; i < dat.size(); i += 4) {
			num = 0;
			num += (unsigned char)dat[i];
			num <<= 8;
			num += (unsigned char)dat[i + 1];
			num <<= 8;
			num += (unsigned char)dat[i + 2];
			num <<= 8;
			num += (unsigned char)dat[i + 3];
			ret += to_string(num);
			ret += "\n";
		}
		return ret;
	}
}
