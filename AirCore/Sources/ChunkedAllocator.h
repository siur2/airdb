#pragma once

#include <string>

namespace AirDB {

using namespace std;

class ChunkedAllocator {
public:
	class Chunk;
private:
	Chunk* head;
	Chunk* tail;
	int chunkCount;
public:
	ChunkedAllocator(void);
	~ChunkedAllocator(void);
	void clear();
	int getChunkCount();
	Chunk* getHead() { return head; }
	Chunk* getTail() { return tail; }
	char* allocate(int count);
	char* allocateCStr(int length);
	char* store(const char* cstr, short length);
	char* store(const string value);
	void shrink(int count);
public:
	class Chunk {
		friend class ChunkedAllocator;
	public:
		static const int CAPACITY = 1 * 1024 * 1024;
	private:
		char* data;
		Chunk* next;
		Chunk* prev;
		int used;
		Chunk();
		~Chunk();
	public:
		void* getData() { return data; }
		int getUsed() { return used; }
	};
};

}