#include <iostream>
#include "FileIO.h"
#include "Config.h"
#include "Generator.h"
#include "Encoder.h"
#include "Decoder.h"
#include "DNA.h"
#include <Windows.h>
using namespace std;

int main() {
	#if IsEncoder
	cout << "[ DNA Encoder ]" << endl << endl;
	#else
	cout << "[ DNA Decoder ]" << endl << endl;
	#endif
	cout << "> DNA Generation System" << endl;
	cout << "  ¤¤DNA Digital Data Storage System Using Methylation and Restriction Modification System" << endl << endl;
	cout << "> 2023 KSA R&E" << endl << endl << endl;
	system("Pause");
	
	cout << "> Select the Files" << endl;
	vector<string> path = FileIO::GetFileNames();
	if (path.empty()) return 0;

	Generator* generator;
	#if IsEncoder
	generator = new Encoder();
	#else
	generator = new Decoder();
	#endif
	generator->Input();
	generator->Generate(path);
	generator->Output();
	delete generator;
	cout << "END" << endl;
	system("Pause");
	while (true) Sleep(1000);
	return 0;
}