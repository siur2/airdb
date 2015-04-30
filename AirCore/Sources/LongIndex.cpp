#include "LongIndex.h"

namespace AirDB {

LongIndex::LongIndex(long long value, long long rowId) {
	this->value = value;
	this->rowId = rowId;
}

bool LongIndex::operator<(const LongIndex& index) const {
	if (value < index.value) {
		return true;
	}
	else if (value > index.value) {
		return false;
	}
	else if (rowId < index.rowId) {
		return true;
	}
	else {
		return false;
	}
}

}