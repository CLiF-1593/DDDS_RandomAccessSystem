#include <iostream>
#include <vector>
#include <FileIO.h>
using namespace std;
int main() {
	cout << "[ DNA Noise Generator ]" << endl << endl;
	cout << "> DNA Random Noise Generation System" << endl;
	cout << "  ¤¤Random Access of DNA Digital Data Storage System Using Restriction Modification System" << endl << endl;
	cout << "> 2023 KSA R&E" << endl << endl << endl;
	system("Pause");

	srand((unsigned)time(NULL));

	cout << "> Select the Files" << endl;
	vector<string> path = FileIO::GetFileNames();
	if (path.empty()) return 0;

	cout << "> Substitution Ratio (%) : ";
	double sub;
	cin >> sub;

	cout << "> Insertion Ratio (%) : ";
	double ins;
	cin >> ins;

	cout << "> Deletion Ratio (%) : ";
	double del;
	cin >> del;

	char nucleotide[4] = { 'A', 'G', 'C', 'T' };

	for (int i = 0; i < path.size(); i++) {
		string p;
		std::string str;
		str.assign(path[i].begin(), path[i].end());
		string dat = FileIO::ReadFileData(str);

		vector<int> arr;
		vector<int> random;
		arr.resize(dat.size());
		for (int i = 0; i < dat.size(); i++) {
			arr[i] = i;
		}
		for (int i = 0; i < (double)dat.size() * sub / 100.0; i++) {
			int index = rand() % arr.size();
			random.push_back(arr[index]);
			arr.erase(arr.begin() + index);
			if (dat[random.back()] == '\n') {
				i--;
				random.pop_back();
			}
		}
		for (int i = 0; i < random.size(); i++) {
			int index = 0;
			for (index = 0; index < 4; index++) if (dat[random[i]] == nucleotide[index]) break;
			index = (index + rand() % 3 + 1) % 4;
			dat[random[i]] = nucleotide[index];
		}
		for (int i = 0; i < (double)dat.size() * ins / 100.0; i++) {
			int index = rand() % dat.size();
			int nuc = rand() % 4;
			dat.insert(dat.begin() + index, nucleotide[nuc]);
		}
		for (int i = 0; i < (double)dat.size() * del / 100.0; i++) {
			int index;
			do {
				index = rand() % dat.size();
			} while (dat[index] == '\n');
			dat.erase(dat.begin() + index);
		}
		while (str.back() != '.') str.pop_back();
		str.pop_back();
		str += "_Err.dna";
		FileIO::SaveFileData(str, dat);
	}
}