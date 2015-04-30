#pragma once

#include "ColumnType.h"

namespace AirDB {

class RecordValue {
private:
	ColumnType type;
	void* value;
private:
	void validate(ColumnType type);
public:
	RecordValue();
	~RecordValue();
	ColumnType getType();
	bool isNull();
	void setInt(int value);
	int getInt();
	void setLong(long long value);
	long long getLong();
	void setDouble(double value);
	double getDouble();
	void setCStr(const char* value);
	const char* getCStr();
};

}