#pragma once

#include "ColumnMetadata.h"

#include <vector>

namespace AirDB {

using namespace std;

class RowMetadata {
private:
	int headSize;
	int bodyColumnCount;
	vector<int> headColumnOffsets;
public:
	RowMetadata(vector<ColumnMetadata>* columns);
	int getHeadSize();
	int getHeadColumnCount();
	int getBodyColumnCount();
	int getHeadColumnOffset(int index);
};

}