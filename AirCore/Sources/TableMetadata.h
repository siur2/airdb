#pragma once

#include "ColumnMetadata.h"
#include "RowMetadata.h"

#include <vector>

namespace AirDB {

using namespace std;

class TableMetadata {
private:
	long long recordCount;
	vector<ColumnMetadata>* columns;
	RowMetadata* rowMetadata;
	int index;
public:
	TableMetadata(const TableMetadata& source) = delete;
	TableMetadata& operator=(TableMetadata value) = delete;

	TableMetadata(vector<ColumnMetadata>* columns, long long recordCount, int index);
	~TableMetadata(void);

	long long getRecordCount() { return recordCount; }
	const vector<ColumnMetadata>& getColumns();
	RowMetadata& getRowMetadata();
	int getIndex();
};

}