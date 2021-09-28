#include "coreFiles.hpp"


int main() {
	setlocale(LC_ALL, "ru");
	fs::path path;
	cout << "Введите директорию:" << endl;
	cin >> path;
	DirFiles dirFiles(path);
	dirFiles.sortFiles();
	dirFiles.allocateFiles();
	return 0;
}
