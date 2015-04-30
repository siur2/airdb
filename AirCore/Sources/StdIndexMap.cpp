#include "StdIndexMap.h"

namespace AirDB {

StdIndexMap::StdIndexMap() {
	rows = new map<LongIndex, char*>();
}

StdIndexMap::~StdIndexMap() {
	delete rows;
}

bool StdIndexMap::begin() {
	rowsIterator = rows->begin();
	rowsEnd = rows->end();
	return rowsIterator != rowsEnd;
}
	
}