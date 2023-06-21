#include <iostream>
#include "FileIO.h"
#include "Config.h"
#include "Generator.h"
#include "Encoder.h"
#include "Decoder.h"
using namespace std;

int main() {
	#if IsEncoder
	cout << "[ Musical Score Encoder ]" << endl << endl;
	#else
	cout << "[ Musical Score Decoder ]" << endl << endl;
	#endif
	cout << "> Musical Score Encoding/Decoding System" << endl;
	cout << "  ¤¤DNA Digital Data Storage System Using Methylation and Restriction Modification System" << endl << endl;
	cout << "  ¤¤Lab2 : Musical Score Encoding" << endl << endl;
	cout << "> 2023 KSA R&E" << endl << endl << endl;
	system("Pause");
	
	Generator* generator;
	#if IsEncoder
	generator = new Encoder();
	#else
	generator = new Decoder();
	#endif
	generator->Input();
	generator->Generate();
	generator->Output();
	delete generator;
	return 0;
}