#include "File.h"

#include <algorithm>
#include <string>

namespace AirDB {

using namespace std;

File::File(const char* fileName, const char* mode) {
	if (fopen_s(&file, fileName, mode)) {
		throw exception(("Failed to open file " + string(fileName)).c_str());
	}
}

File::~File() {
	fflush(file);
	fclose(file);
}

void File::read(void* buf, int count) {
	size_t actualCount = fread(buf, 1, count, file);
	if (actualCount < count) {
		throw exception("Failed to read requested amount of data from file");
	}
}

void File::read(long long address, void* buf, int count) {
	_fseeki64(file, address, SEEK_SET);
	read(buf, count);
}

void File::write(void* buf, int count) {
	size_t actualCount = fwrite(buf, count, 1, file);
	if (actualCount != 1) {
		throw exception("Failed to write data to file");
	}
}

void File::write(long long address, void* buf, int count) {
	_fseeki64(file, address, SEEK_SET);
	write(buf, count);
}

long long File::append(void* buf, int count) {
	_fseeki64(file, 0, SEEK_END);
	long long result = _ftelli64(file);
	fwrite(buf, count, 1, file);
	return result;
}

bool File::exists(const char* fileName) {
	FILE* file;
	if (!fopen_s(&file, fileName, "r")) {
		fclose(file);
	}
	return file != 0;
}

bool File::del(const char* fileName) {
	return remove(fileName) == 0;
}

void File::delVerify(const char* fileName) {
	if (remove(fileName) != 0) {
		string msg("Failed to delete file ");
		msg += fileName;
		throw exception(msg.c_str());
	}
}

}