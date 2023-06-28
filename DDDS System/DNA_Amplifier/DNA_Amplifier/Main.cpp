#include <iostream>
#include <vector>
#include <FileIO.h>
using namespace std;
int main() {
	cout << "[ DNA Amplifier ]" << endl << endl;
	cout << "> DNA Amplifying System" << endl;
	cout << "  ¤¤Random Access of DNA Digital Data Storage System Using Restriction Modification System" << endl << endl;
	cout << "> 2023 KSA R&E" << endl << endl << endl;
	system("Pause");

	srand((unsigned)time(NULL));

	cout << "> Select the Files" << endl;
	vector<string> path = FileIO::GetFileNames();
	if (path.empty()) return 0;

	cout << "> Number of Amplification : ";
	int num;
	cin >> num;

	char nucleotide[4] = { 'A', 'G', 'C', 'T' };

	for (int i = 0; i < path.size(); i++) {
		string dat = FileIO::ReadFileData(path[i]);
		if (dat.back() != '\n') dat += "\n";
		for (int i = 0; i < num; i++) {
			dat += dat;
		}
		FileIO::SaveFileData(path[i], dat);
	}
}