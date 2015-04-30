#include "RowMetadata.h"

namespace AirDB {

RowMetadata::RowMetadata(vector<ColumnMetadata>* columns) : headColumnOffsets() {
	headSize = 0;

	bool headEnded = false;
	for (int i = 0; i < columns->size(); i++) {
		ColumnMetadata& column = (*columns)[i];
		bool headColumn = column.getType() != ColumnType::STRING;
		if (headColumn) {
			if (headEnded) {
				throw exception("Invalid column order");
			}

			headColumnOffsets.push_back(headSize);
			headSize += column.getColumnSize();
		} else {
			headEnded = true;
		}
	}

	bodyColumnCount = (int)(columns->size() - headColumnOffsets.size());
}

int RowMetadata::getHeadSize() {
	return headSize;
}

int RowMetadata::getHeadColumnCount() {
	return (int)headColumnOffsets.size();
}

int RowMetadata::getBodyColumnCount() {
	return bodyColumnCount;
}

int RowMetadata::getHeadColumnOffset(int index) {
	return headColumnOffsets[index];
}

}