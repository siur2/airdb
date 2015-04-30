#pragma once

#include "TableMetadata.h"
#include "ChunkedAllocator.h"
#include "BufferedFileReader.h"
#include "BufferedFileWriter.h"
#include "LongIndex.h"
#include "IndexMap.h"

namespace AirDB {

class Table {
private:
	TableMetadata* metadata;
	ChunkedAllocator* allocator;
	ChunkedAllocator* bodyAllocator;
	IndexMap* index;
public:
	Table(void);
	~Table(void);
	TableMetadata& getMetadata();
	int getRowCount();
	int count();
	void init(TableMetadata* metadata);
	int readFromFile(const char* fileName);
	int saveToFile(const char* fileName);
	double sum(int columnIndex);
	void addRow(const char* head, const vector<char*>& body);
private:
	void readRowdata(BufferedFileReader* reader);
	void writeRowdataB(BufferedFileWriter* writer);
	double sumDouble(int columnIndex);
	double sumInt(int columnIndex);
};

}