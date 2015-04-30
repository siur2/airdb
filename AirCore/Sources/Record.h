#pragma once

#include <vector>

#include "RecordValue.h"

namespace AirDB {

using namespace std;

class Record {
private:
	vector<RecordValue>* values;
public:
	Record(int columnCount);
	~Record();

	vector<RecordValue>* getValues() { return values; }
};

}