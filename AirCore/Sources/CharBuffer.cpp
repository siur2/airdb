#include "CharBuffer.h"

#include <algorithm>

namespace AirDB {

using namespace std;

CharBuffer::CharBuffer(int size) {
	this->size = size;
	charsWritten = 0;
	data = new char[this->size];
}

CharBuffer::~CharBuffer() {
	delete[] data;
}

int CharBuffer::getSize() {
	return size;
}

const char* CharBuffer::getData() {
	return data;
}

void CharBuffer::write(char* buf, int count) {
	if (charsWritten + count > size) {
		throw exception("Not enough free space in buffer");
	}

	memcpy(data + charsWritten, buf, count);
	charsWritten += count;
}

void CharBuffer::reset() {
	charsWritten = 0;
}

}