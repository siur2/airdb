#include "BufferedFileWriter.h"

#include <algorithm>
#include <string>

namespace AirDB {

using namespace std;

BufferedFileWriter::BufferedFileWriter(const char* fileName, const char* mode) {
	bufferWritten = 0;
	size = 0;
	buffer = new char[CAPACITY];
	if (fopen_s(&file, fileName, mode)) {
		throw exception(("Failed to open file " + string(fileName)).c_str());
	}
}

BufferedFileWriter::~BufferedFileWriter(void) {
	flush();
	delete[] buffer;

	fflush(file);
	fclose(file);
}

void BufferedFileWriter::write(const char* buf, int count) {
	if (count <= 0) {
		return;
	}

	int available = CAPACITY - bufferWritten;
	if (available >= count) {
		memcpy(buffer + bufferWritten, buf, count);
		bufferWritten += count;
	} else {
		if (available > 0) {
			memcpy(buffer + bufferWritten, buf, available);
			bufferWritten = CAPACITY;
		}
		flush();
		bufferWritten = count - available;
		if (bufferWritten > CAPACITY) {
			throw exception("Buffer overflow!"); //TODO: implement w/o overflow
		}
		memcpy(buffer, buf + available, count - available);
	}
	size += count;
}

void BufferedFileWriter::flush() {
	if (bufferWritten == 0) {
		return;
	}

	fwrite(&buffer[0], bufferWritten, 1, file);
	bufferWritten = 0;
}

void BufferedFileWriter::writeCStr(const char* value) {
	short length = *(short*)(&value[-(int)sizeof(length)]);
	write(&value[-(int)sizeof(length)], length + sizeof(length));
}

}