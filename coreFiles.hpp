#include <iostream>
#include <vector>
#include <filesystem>
#include <string>
#include <clocale>

using namespace std;
namespace fs = std::filesystem;

const int maxDirSize = 100;

bool errorHandler(error_code);

class File {
public:
	fs::path path;
	uintmax_t size;
	
	File(fs::path);
private:
	error_code ec;
	uintmax_t getSize();
};


class DirFiles {
public:
	DirFiles(fs::path);
	void allocateFiles();
	void sortFiles();
private:
	vector<File> files;
	fs::path path;
	error_code ec;

	void getFiles();
	void createDir(fs::path);
	bool moveFile(fs::path, fs::path);
};