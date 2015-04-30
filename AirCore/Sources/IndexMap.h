#pragma once

#include "LongIndex.h"

namespace AirDB {

class IndexMap {
public:
	virtual ~IndexMap() {}
	virtual void add(LongIndex key, char* data) = 0;
	virtual bool begin() = 0;
	virtual bool next() = 0;
	virtual LongIndex getKey() = 0;
	virtual char* getData() = 0;
	virtual int getSize() = 0;
};

}