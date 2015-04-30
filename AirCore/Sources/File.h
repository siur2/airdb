#pragma once

#include <stdio.h>

namespace AirDB {

class File {
private:
	FILE* file;
public:
	File(const char* fileName) : File(fileName, "r+b") {};
	File(const char* fileName, const char* mode);
	~File();
	void read(void* buf, int count);
	void read(long long address, void* buf, int count);
	void write(void* buf, int count);
	void write(long long address, void* buf, int count);
	long long append(void* buf, int count);
	static bool exists(const char* fileName);
	static bool del(const char* fileName);
	static void delVerify(const char* fileName);
};

}