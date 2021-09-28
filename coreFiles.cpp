#include "coreFiles.hpp"

bool errorHandler(error_code ec) {
	if (ec) {
		cout << "\033[31m[Error] \033[0m" << ec.message() << endl;
		return true;
	}
	return false;
}

File::File(fs::path p) {
	path = p;
	size = getSize();
}

uintmax_t File::getSize() {
	uintmax_t temp_size = fs::file_size(path, ec);
	if (!errorHandler(ec))
		return temp_size / 1024;
	else
		return 0;
}

DirFiles::DirFiles(fs::path p) {
	path = p;
	getFiles();
}

void DirFiles::getFiles() {
	for (auto const& entry : fs::directory_iterator{ path }) {
		if (!fs::is_directory(entry, ec)) {
			if (!errorHandler(ec))
				files.push_back(fs::canonical(entry.path(), ec));
		}
	}
}

void DirFiles::createDir(fs::path newDirPath) {
	fs::create_directory(newDirPath, ec);
	errorHandler(ec);
}

bool DirFiles::moveFile(fs::path oldPath, fs::path newPath) {
	fs::rename(oldPath, newPath, ec);
	if (!errorHandler(ec)) {
		newPath = fs::canonical(newPath);
		cout << oldPath.filename().string() << "\033]32m moved to \033]0m" << newPath << endl;
		return true;
	}
	return false;
}

void DirFiles::allocateFiles() {
	uintmax_t sizeNow = 0;
	for (int i = 0, folder = 1; i < files.size(); i++, folder++)
	{
		fs::path newDirPath = path.string() + "/Dir " + to_string(folder) + "/";
		createDir(newDirPath);
		if (files[i].size <= maxDirSize) {
			while (sizeNow + files[i].size <= maxDirSize) {
				fs::path newPath = newDirPath.string() + "/" + files[i].path.filename().string();
				if (moveFile(files[i].path, newPath)) {
					sizeNow += files[i].size;
					i++;
				}
			}
			if (sizeNow + files[i].size > maxDirSize)
				i--;
			sizeNow = 0;
		}
		else if (files[i].size > maxDirSize) {
			fs::path newPath = newDirPath.string() + "/" + files[i].path.filename().string();
			moveFile(files[i].path, newPath);
		}
	}
}

void DirFiles::sortFiles() {
	for (int i = 0; i < files.size() - 1; i++)
	{
		for (int j = 0; j < files.size() - 1; j++)
		{
			if (files[j].size > files[j + 1].size) {
				File tempFile = files[j];
				files[j] = files[j + 1];
				files[j + 1] = tempFile;
			}
		}
	}
}