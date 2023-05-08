#include <iostream>
#include <string>
#include <fstream>
#include <direct.h>
#include <windows.h>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);
WORD oldclr;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;
const int RED = 12, GREEN = 10;
const char NO = 'X', YES = '+', NONE = ' ';

string tClr(int clrid, char sym) {
	if (sym == ' ') {
		SetConsoleTextAttribute(h, (clrid == -1 ? oldclr : clrid));
		return "";
	}
	cout << "[ ";
	SetConsoleTextAttribute(h, clrid);
	cout << sym;
	SetConsoleTextAttribute(h, oldclr);
	cout << " ] ";
	return "";
}

void drawProgress(float progress, int width) {
	cout << "[";
	progress < 1.0 ? tClr(8, NONE) : tClr(2, NONE);
	int pos = width * progress;
	for (int j = 0; j < width; ++j) {
		if (j < pos) cout << "#";
		else cout << " ";
	}
	tClr(-1, NONE);
	cout << "] " << (progress > 1.0 ? 100 : int(progress * 100.0)) << " %\r";
	cout.flush();
}

void generatePart(string dir, string name, int id, char* bits, int bitssize) {
	ofstream temp(dir + "\\" + name + "^" + to_string(float(id) / 1000000).substr(2, 6), std::ios::out | std::ios::binary | std::ios::ate);
	if (!temp.is_open()) return;
	temp.write(&bits[0], bitssize);
	temp.close();
}

void main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	GetConsoleScreenBufferInfo(h, &csbiInfo);
	oldclr = csbiInfo.wAttributes;
	string path;
	cout << "[     шлях до файлу\\-> ";
	cin >> path;
	int wei;
	cout << "[     Поділити на КБ\\-> ";
	cin >> wei;
	wei *= 1000;
	ifstream file(path, std::ios::binary);
	if (!file.is_open()) {
		cout << tClr(RED, NO) << "Файл '" << path << "' не можливо відкрити" << endl;
		system("pause");
		return;
	}
	file.seekg(0, ios::end);
	long long size = file.tellg();
	if (size <= 0) {
		cout << tClr(RED, NO) << "Файл '" << path << "' має розмір: " << size << "біт, він порожній" << endl;
		system("pause");
		return;
	}
	file.seekg(0, ios::beg);
	bool haveremain = (size % wei != 0 ? true : false);
	int parts = size / wei;
	string pathnew = (path + " - " + to_string(parts + haveremain) + "^" + to_string(wei / 1000) + "КБ");
	char* bufchar = new char[wei];
	_mkdir(pathnew.c_str());
	float progress = 0.0;
	for (int i = 0; i < parts; i++) {
		drawProgress(progress, 45);
		file.read(bufchar, wei);
		generatePart(pathnew, path, i, bufchar, wei);
		file.seekg(file.tellg());
		progress += (100 / float(parts)) / 100;
	}
	drawProgress(1, 45);
	cout << "\n";
	cout << tClr(GREEN, YES) << "Розділення завершено!" << endl;
	if (haveremain) {
		int lsize = size - wei * parts;
		file.read(bufchar, lsize);
		generatePart(pathnew, path, parts, bufchar, lsize);
	}
	cout << tClr(GREEN, YES) << "Обробка залишка завершена!" << endl;
	ifstream origun("uniter.exe", ios::binary);
	if (!origun.is_open()) {
		cout << tClr(RED, NO) << "Не знайден файл 'uniter.exe', об'єднання не буде працювати." << endl;
		system("pause");
		return;
	}
	ofstream un(pathnew + "\\" + path + "^" + to_string(parts + haveremain) + "-uniter.exe", std::ios::out | std::ios::binary | std::ios::ate);
	un << string((istreambuf_iterator<char>(origun)), istreambuf_iterator<char>());
	un.close();
	cout << tClr(GREEN, YES) << "Файл об'єднання створений успішно!" << endl;
	system("pause");
}

