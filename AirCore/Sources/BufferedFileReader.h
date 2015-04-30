#pragma once

#include <stdio.h>
#include "ChunkedAllocator.h"

namespace AirDB {

class BufferedFileReader
{
private:
	static const int CAPACITY = 1024 * 1024;
private:
	char* buffer;
	ChunkedAllocator* allocator;
	long size;
	int bufferRead;
	int bufferWritten;
	FILE* file;
public:
	BufferedFileReader(const char* fileName, ChunkedAllocator* allocator);
	~BufferedFileReader(void);
	void read(char* dest, int count);
	bool eof();
	char* readCStr();
};

}