#include "BufferedFileReader.h"

#include <algorithm>

namespace AirDB {

using namespace std;

BufferedFileReader::BufferedFileReader(const char* fileName, ChunkedAllocator* allocator) {
	this->allocator = allocator;

	size = 0;
	bufferRead = 0;
	bufferWritten = 0;
	buffer = new char[CAPACITY];

	if (fopen_s(&file, fileName, "rb")) {
		throw exception(("Failed to open file " + string(fileName)).c_str());
	}
}

BufferedFileReader::~BufferedFileReader(void) {
	delete[] buffer;
	fclose(file);
}

void BufferedFileReader::read(char* dest, int count) {
	if (count <= 0) {
		return;
	}

	int available = bufferWritten - bufferRead;
	if (available >= count) {
		memcpy(dest, &buffer[bufferRead], count);
		bufferRead += count;
	} else {
		if (available > 0) {
			memcpy(dest, &buffer[bufferRead], available);
		}
		
		bufferWritten = (int)fread(&buffer[0], 1, CAPACITY, file);
		size += bufferWritten;

		bufferRead = count - available;
		if (bufferRead > bufferWritten) {
			throw exception("End of buffer");
		}
		memcpy(&dest[available], &buffer[0], bufferRead);
	}
}

char* BufferedFileReader::readCStr() {
	short length;
	read((char*)&length, sizeof(length));
	char* result = allocator->allocateCStr(length);
	read(result, length);
	return result;
}

bool BufferedFileReader::eof() {
	return (feof(file) != 0) && (bufferRead == bufferWritten);
}

}