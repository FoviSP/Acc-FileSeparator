#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <windows.h>

using namespace std;

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	string dat;
	string path;
	cout << "шлях до файлу\\-> ";
	cin >> path;
	ifstream file(path, std::ios::binary);
	if (!file.is_open()) return;
	size_t fsize = file.tellg();
	if (fsize < 0) return;
	file.seekg(0, std::ios::beg);
	dat = string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	cout << "Файл успішно завантажен!" << endl;
	int wei;
	cout << "Поділити на КБ\\-> ";
	cin >> wei;
	wei *= 1000;
	bool haveremain = false;
	int parts = dat.length() / wei;
	dat.length() % wei != 0 ? haveremain = true : haveremain = false;
	string pathnew = (path + " - " + to_string(parts + haveremain) + "^" + to_string(wei / 1000) + "КБ");
	_mkdir(pathnew.c_str());
	for (int i = 0; i < parts; i++) {
		ofstream temp(pathnew + "\\" + path + "^" + to_string(float(i) / 1000000).substr(2, 6), std::ios::out | std::ios::binary | std::ios::ate);
		if (!file.is_open()) return;
		temp << dat.substr(i*wei, wei);
		temp.close();
	}
	if (haveremain) {
		ofstream temp(pathnew + "\\" + path + "^" + to_string(float(parts) / 1000000).substr(2, 6), std::ios::out | std::ios::binary | std::ios::ate);
		if (!file.is_open()) return;
		temp << dat.substr(parts*wei);
		temp.close();
	}
	ofstream un(pathnew + "\\" + path + "^" + to_string(parts + haveremain) + "-uniter.exe", std::ios::out | std::ios::binary | std::ios::ate);
	ifstream origun("Acc-FileUniter.exe", std::ios::binary);
	un << string((std::istreambuf_iterator<char>(origun)), std::istreambuf_iterator<char>());
	un.close();


}