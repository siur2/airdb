#include "ChunkedAllocator.h"
#include <algorithm>

namespace AirDB {

using namespace std;

ChunkedAllocator::ChunkedAllocator(void) {
	head = new Chunk();
	tail = head;
	chunkCount = 1;
}

ChunkedAllocator::~ChunkedAllocator(void) {
	while (head != 0) {
		Chunk* next = head->next;
		delete head;
		head = next;
	}
	tail = 0;
}

void ChunkedAllocator::clear() {
	while (head != tail) {
		Chunk* next = head->next;
		delete head;
		head = next;
	}
	tail->prev = 0;
	tail->used = 0;
	chunkCount = 1;
}

int ChunkedAllocator::getChunkCount() {
	return chunkCount;
}

char* ChunkedAllocator::allocate(int count) {
	if (count <= 0) {
		return 0;
	}

	if (tail->used + count > Chunk::CAPACITY) {
		tail->next = new Chunk();
		tail->next->prev = tail;
		tail = tail->next;
		chunkCount++;
	}
	char* result = &tail->data[tail->used];
	tail->used += count;
	return result;
}

char* ChunkedAllocator::allocateCStr(int length) {
	if (length < 0) {
		return 0;
	}

	int stringSize = length + 3;
	if (tail->used + stringSize > Chunk::CAPACITY) {
		tail->next = new Chunk();
		tail->next->prev = tail;
		tail = tail->next;
		chunkCount++;
	}
	long start = tail->used;
	tail->used += stringSize;
	*(short*)(&tail->data[start]) = length;
	tail->data[tail->used - 1] = 0;
	return &tail->data[start + 2];
}

char* ChunkedAllocator::store(const char* cstr, short length) {
	char* result = allocateCStr(length);
	memcpy(result, cstr, length);
	return result;
}

char* ChunkedAllocator::store(const string value) {
	short length = (short)value.length();
	const char* cstr = value.c_str();
	return store(cstr, length);
}

void ChunkedAllocator::shrink(int count) {
	if (count > tail->used) {
		throw exception("Can not shrink below tail used space");
	}

	tail->used -= count;
	Chunk* prev = tail->prev;
	if ((tail->used == 0) && (prev != 0)) {
		delete tail;
		tail = prev;
		tail->next = 0;
		chunkCount--;
	} else {
		memset(&tail->data[tail->used], 0, count);
	}
}

ChunkedAllocator::Chunk::Chunk() {
	next = 0;
	prev = 0;
	used = 0;
	data = (char*)calloc(1, CAPACITY);
}

ChunkedAllocator::Chunk::~Chunk() {
	free(data);
}

}