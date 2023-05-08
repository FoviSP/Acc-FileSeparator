#include <string>
#include <fstream>
#include <windows.h>

using namespace std;

void main(int argc, char** argv) {
	size_t fsize;
	string apath = argv[0];
	string aname = apath.substr(apath.find_last_of('\\') + 1);
	if (aname.find('^') == string::npos || aname.find('\-') == string::npos) {
		printf("Don't touch changeling equestrian cunt!\n");
		return;
	}
	string path = aname.substr(0, aname.find('^'));
	int amount = stoi(aname.substr(aname.find_last_of('^') + 1, aname.find_last_of('\-')));
	ofstream output(path, std::ios::out | std::ios::binary | std::ios::ate);
	if (!output.is_open()) return;
	string bufname;
	for (int i = 0; i < amount; i++) {
		bufname = path + "^" + to_string(float(i) / 1000000).substr(2, 6);
		ifstream file(bufname, std::ios::binary);
		fsize = file.tellg();
		if (fsize < 0) return;
		file.seekg(0, std::ios::beg);
		output << string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
		file.close();
		remove(bufname.c_str());
	}
	output.close();
	ofstream delbat("_del.bat", std::ios::binary);
	delbat << "@echo off\r\n:loop\r\ndel \"" + aname + "\"\r\nif exist \"" + aname + "\" goto :loop\r\ndel %0";
	delbat.close();
	ShellExecute(NULL, "open", "_del.bat", NULL, NULL, NULL);
}