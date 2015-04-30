#pragma once

#include "Table.h"
#include "RecordValue.h"

namespace AirDBImport {

using namespace System;
using namespace System::Data::SqlClient;

using namespace AirDB;

class ImportController {
private:
	ColumnType getColumnType(System::String^ type);
	TableMetadata* createMetadata(System::String^ metadataJson);
	long long importRows(const char* fileName, TableMetadata* metadata, SqlDataReader^ reader, int recordsLimit);
	void setRecordValue(ColumnType type, Object^ obj, RecordValue* value);
public:
	ImportController();
	~ImportController();
	long long import(String^ connectionString, String^ tableName, String^ tableMetadataJson, int recordsLimit);
};

}