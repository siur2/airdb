#pragma once

namespace AirDB {

class LongIndex {
private:
	long long value;
	long long rowId;
public:
	LongIndex(long long value, long long rowId);
	bool operator<(const LongIndex& index) const;
};

}