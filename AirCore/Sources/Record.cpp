#include "Record.h"

namespace AirDB {

Record::Record(int columnCount) {
	this->values = new vector<RecordValue>(columnCount);
}

Record::~Record() {
	delete values;
}

}