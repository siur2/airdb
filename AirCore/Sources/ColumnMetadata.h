#pragma once

#include "ColumnType.h"
#include <string>

namespace AirDB {

using namespace std;

class ColumnMetadata {
private:
	ColumnType type;
	string name;
public:
	ColumnMetadata();
	void init(ColumnType type, const char* name);
	ColumnType getType() const;
	const char* getName() const;
	int getColumnSize();
	static string getColumnTypeName(ColumnType type);
};

}