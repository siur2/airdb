#pragma once

#include "IndexMap.h"

#include <map>

namespace AirDB {

using namespace std;

class StdIndexMap : public IndexMap {
private:
	map<LongIndex, char*>* rows;
	map<LongIndex, char*>::const_iterator rowsIterator;
	map<LongIndex, char*>::const_iterator rowsEnd;
public:
	StdIndexMap();
	~StdIndexMap();
	void add(LongIndex key, char* data) { rows->insert(pair<LongIndex, char*>(key, data)); }
	int getSize() { return (int)rows->size(); }
	bool begin();
	bool next() { rowsIterator++; return rowsIterator != rowsEnd; }
	LongIndex getKey() { return rowsIterator->first; }
	char* getData() { return rowsIterator->second; }
};

}