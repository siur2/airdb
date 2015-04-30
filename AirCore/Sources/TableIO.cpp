#include "TableIO.h"

namespace AirDB {

void TableIO::createTable(const char* fileName, TableMetadata* metadata) {
	BufferedFileWriter writer(fileName);
	writeMetadata(&writer, metadata);
}

TableMetadata* TableIO::readMetadata(BufferedFileReader* reader) {
	int index;
	int columnCount;
	long long recordCount;
	reader->read((char*)&recordCount, sizeof(recordCount));
	reader->read((char*)&columnCount, sizeof(columnCount));
	unique_ptr<vector<ColumnMetadata>> columns(new vector<ColumnMetadata>(columnCount));
	for (auto& column : *columns) {
		ColumnType columnType;
		reader->read((char*)&columnType, sizeof(columnType));
		char* columnName = reader->readCStr();
		column.init(columnType, columnName);
	}

	reader->read((char*)&index, sizeof(index));
	if ((*columns)[index].getType() != ColumnType::LONG) {
		throw exception("Index is supported only for column of type long");
	}
	return new TableMetadata(columns.release(), recordCount, index);
}

void TableIO::writeMetadata(BufferedFileWriter* writer, TableMetadata* metadata) {
	const vector<ColumnMetadata>& columns = metadata->getColumns();
	long long recordCount = metadata->getRecordCount();
	int columnCount = (int)columns.size();
	writer->write((char*)&recordCount, sizeof(recordCount));
	writer->write((char*)&columnCount, sizeof(columnCount));

	for (auto& column : columns) {
		ColumnType columnType = column.getType();
		writer->write((char*)&columnType, sizeof(columnType));
		const char* columnName = column.getName();
		short columnNameSize = (short)strlen(columnName);
		writer->write((char*)&columnNameSize, sizeof(columnNameSize));
		writer->write(columnName, columnNameSize);
	}

	int index = metadata->getIndex();
	writer->write((char*)&index, sizeof(index));
}

void TableIO::appendRowdata(const char* fileName, vector<unique_ptr<Record>>* records) {
	ChunkedAllocator allocator;
	unique_ptr<BufferedFileReader> reader(new BufferedFileReader(fileName, &allocator));
	unique_ptr<TableMetadata> metadata(readMetadata(reader.get()));
	reader.reset();

	RowMetadata& rowMetadata = metadata->getRowMetadata();
	const vector<ColumnMetadata>& columns = metadata->getColumns();
	int headColumnCount = rowMetadata.getHeadColumnCount();
	unique_ptr<BufferedFileWriter> writer(new BufferedFileWriter(fileName, "ab"));

	for (auto& it : *records) {
		vector<RecordValue>* values = it.get()->getValues();
		if (values->size() != columns.size()) {
			throw exception("Record size does not match metadata");
		}

		int bodySize = calcBodySize(values, headColumnCount);
		writer.get()->write((char*)&bodySize, sizeof(bodySize));

		for (unsigned int i = 0; i < columns.size(); i++) {
			RecordValue* value = &(*values)[i];
			if (value->getType() != columns[i].getType()) {
				throw exception("Record value type does not match metadata");
			}
			writeRecordValue(writer.get(), value);
		}
	}

	writer.reset();
	writer.reset(new BufferedFileWriter(fileName, "r+b"));
	long long recordCount = metadata->getRecordCount() + records->size();
	writer.get()->write((char*)&recordCount, sizeof(recordCount));
}

void TableIO::writeRecordValue(BufferedFileWriter* writer, RecordValue* value) {
	switch (value->getType()) {
	case INT: {
		int intValue = value->getInt();
		writer->write((char*)&intValue, sizeof(intValue));
		break;
	}
	case LONG: {
		long long longValue = value->getLong();
		writer->write((char*)&longValue, sizeof(longValue));
		break;
	}
	case DOUBLE: {
		double doubleValue = value->getDouble();
		writer->write((char*)&doubleValue, sizeof(doubleValue));
		break;
	}
	case STRING: {
		const char* strValue = value->getCStr();
		short length = (short)strlen(strValue);
		writer->write((char*)&length, sizeof(length));
		writer->write(strValue, length);
	}
	}
}

int TableIO::calcBodySize(vector<RecordValue>* values, int headColumnCount) {
	int result = 0;
	for (unsigned int i = headColumnCount; i < values->size(); i++) {
		const char* cstr = (*values)[i].getCStr();
		short length = (short)strlen(cstr);
		result += length + sizeof(length);
	}
	return result;
}

}