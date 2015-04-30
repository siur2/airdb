#pragma once

namespace AirDB {

class CharBuffer {
private:
	int size;
	int charsWritten;
	char* data;
public:
	CharBuffer(int size);
	~CharBuffer();
	int getSize();
	const char* getData();
	void write(char* buf, int count);
	void reset();
};

}