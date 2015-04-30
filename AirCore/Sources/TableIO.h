#pragma once

#include "TableMetadata.h"
#include "BufferedFileReader.h"
#include "BufferedFileWriter.h"
#include "Record.h"

#include <memory>

namespace AirDB {

class TableIO {
	static void writeRecordValue(BufferedFileWriter* writer, RecordValue* value);
	static int calcBodySize(vector<RecordValue>* values, int headColumnCount);
public:
	static void createTable(const char* fileName, TableMetadata* metadata);
	static TableMetadata* readMetadata(BufferedFileReader* reader);
	static void writeMetadata(BufferedFileWriter* writer, TableMetadata* metadata);
	static void appendRowdata(const char* fileName, vector<unique_ptr<Record>>* records);
};

}