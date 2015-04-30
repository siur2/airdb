#include "TableMetadata.h"

namespace AirDB {

TableMetadata::TableMetadata(vector<ColumnMetadata>* columns, long long recordCount, int index) {
	this->rowMetadata = new RowMetadata(columns);
	this->columns = columns;
	this->recordCount = recordCount;
	this->index = index;
}

TableMetadata::~TableMetadata(void) {
	delete rowMetadata;
	delete columns;
}

const vector<ColumnMetadata>& TableMetadata::getColumns() {
	return *columns;
}

RowMetadata& TableMetadata::getRowMetadata() {
	return *rowMetadata;
}

int TableMetadata::getIndex() {
	return index;
}

}