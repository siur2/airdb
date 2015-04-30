#pragma once

#include <stdio.h>

namespace AirDB {

class BufferedFileWriter
{
private:
	static const int CAPACITY = 1024 * 1024;
private:
	char* buffer;
	int bufferWritten;
	long size;
	FILE* file;
public:
	BufferedFileWriter(const char* fileName) : BufferedFileWriter(fileName, "wb") {}
	BufferedFileWriter(const char* fileName, const char* mode);
	~BufferedFileWriter(void);
	void write(const char* buf, int count);
	void writeCStr(const char* value);
private:
	void flush();
};

}