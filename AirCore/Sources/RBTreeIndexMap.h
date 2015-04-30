#pragma once

#include "IndexMap.h"
#include "RBTree.h"

namespace AirDB {

class RBTreeIndexMap : public IndexMap {
private:
	RBTree* data;
	RBTree::Iterator* iterator;
	static bool compare(void* left, void* right);
public:
	RBTreeIndexMap();
	~RBTreeIndexMap();
	void add(LongIndex key, char* data);
	bool begin();
	bool next() { return iterator->next(); }
	LongIndex getKey();
	char* getData() { return ((Item*)iterator->getData())->data; }
	int getSize();
private:
	class Item {
		friend class RBTreeIndexMap;
		LongIndex key;
		char* data;
		Item(LongIndex key, char* data) : key(key), data(data) {};
	};
};

}