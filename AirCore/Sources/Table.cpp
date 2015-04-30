#include "Table.h"
#include "TableIO.h"
#include "StdIndexMap.h"
#include "RBTreeIndexMap.h"

#include <ctime>
#include <memory>

namespace AirDB {

Table::Table(void) {
	metadata = 0;
	allocator = new ChunkedAllocator();
	bodyAllocator = new ChunkedAllocator();
	//index = new StdIndexMap();
	index = new RBTreeIndexMap();
}

Table::~Table(void) {
	delete metadata;
	delete allocator;
	delete bodyAllocator;
	delete index;
}

TableMetadata& Table::getMetadata() {
	return *metadata;
}

int Table::getRowCount() {
	return index->getSize();
}

void Table::init(TableMetadata* metadata) {
	if (this->metadata) {
		throw exception("Table already initialized");
	}

	this->metadata = metadata;
}

int Table::readFromFile(const char* fileName) {
	if (metadata) {
		throw exception("Table already initialized");
	}

	clock_t startTime = clock();
	BufferedFileReader reader(fileName, allocator);
	metadata = TableIO::readMetadata(&reader);
	readRowdata(&reader);
	return clock() - startTime;
}

void Table::readRowdata(BufferedFileReader* reader) {
	RowMetadata& rowMetadata = metadata->getRowMetadata();
	int indexOffset = rowMetadata.getHeadColumnOffset(metadata->getIndex());
	int headSize = rowMetadata.getHeadSize();
	int bodyColumnCount = rowMetadata.getBodyColumnCount();
	int bodyHeaderSize = bodyColumnCount * sizeof(int);
	for (int i = 0; !reader->eof(); i++) {
		int bodySize = 0;
		reader->read((char*)&bodySize, sizeof(bodySize));
		char* rowData = allocator->allocate(headSize + sizeof(char*));
		reader->read(rowData, headSize);
		char* bodyData = bodyAllocator->allocate(bodyHeaderSize + bodySize);
		*(char**)(rowData + headSize) = bodyData;

		long long indexValue = *(long long *)(rowData + indexOffset);
		LongIndex index = LongIndex(indexValue, i);
		this->index->add(index, rowData);

		int columnOffset = 0;
		char* columnData = bodyData + bodyHeaderSize;
		for (int j = 0; j < bodyColumnCount; j++) {
			reader->read(columnData, sizeof(short));
			short columnSize = *(short*)columnData;
			columnData += sizeof(short);
			reader->read(columnData, columnSize);
			columnData += columnSize;
			*columnData = 0;
			columnData += sizeof(char);

			*(int *)(bodyData + j * sizeof(int)) = columnOffset;
			columnOffset += columnSize + sizeof(short) + sizeof(char);
		}
	}
}

int Table::saveToFile(const char* fileName) {
	if (!metadata) {
		throw exception("Table not initialized");
	}

	clock_t startTime = clock();
	BufferedFileWriter writer(fileName);
	TableIO::writeMetadata(&writer, metadata);
	writeRowdataB(&writer);
	return clock() - startTime;
}

void Table::writeRowdataB(BufferedFileWriter* writer) {
	RowMetadata& rowMetadata = metadata->getRowMetadata();
	int bodyColumnCount = rowMetadata.getBodyColumnCount();
	int bodyHeaderSize = bodyColumnCount * sizeof(int);
	int headSize = rowMetadata.getHeadSize();
	if (index->begin()) {
		do {
			char* head = index->getData();
			char* body = *(char**)(head + headSize);
			int lastColumnOffset = *(int*)(body + (bodyColumnCount - 1) * sizeof(int));
			int bodySize = lastColumnOffset + *(short*)(body + bodyHeaderSize + lastColumnOffset) +
				sizeof(short) + sizeof(char);
			writer->write((char*)&bodySize, sizeof(bodySize));

			writer->write(head, headSize);

			for (int j = 0; j < bodyColumnCount; j++) {
				int columnOffset = *(int*)(body + j * sizeof(int));
				char* columnData = body + bodyHeaderSize + columnOffset;
				short columnSize = *(short*)(columnData);
				writer->write(columnData, columnSize + sizeof(short));
			}
		} while (index->next());
	}
}

void Table::addRow(const char* head, const vector<char*>& body) {
	RowMetadata& rowMetadata = metadata->getRowMetadata();
	int headSize = rowMetadata.getHeadSize();
	char* rowData = allocator->allocate(headSize + sizeof(char*));
	memcpy(rowData, head, headSize);

	int bodyColumnCount = rowMetadata.getBodyColumnCount();
	int bodyHeaderSize = bodyColumnCount * sizeof(int);
	int bodySize = 0;
	for (int i = 0; i < bodyColumnCount; i++) {
		bodySize += (int)strlen(body[i]) + sizeof(short) + sizeof(char);
	}

	char* bodyData = bodyAllocator->allocate(bodyHeaderSize + bodySize);
	*(char**)(rowData + headSize) = bodyData;

	int columnOffset = 0;
	char* columnData = bodyData + bodyHeaderSize;
	for (int i = 0; i < bodyColumnCount; i++) {
		short columnSize = (short)strlen(body[i]);
		memcpy(columnData, &columnSize, sizeof(columnSize));
		columnData += sizeof(columnSize);
		memcpy(columnData, body[i], columnSize);
		columnData += columnSize;
		*columnData = 0;
		columnData += sizeof(char);

		*(int *)(bodyData + i * sizeof(int)) = columnOffset;
		columnOffset += columnSize + sizeof(columnSize) + sizeof(char);
	}

	int indexOffset = rowMetadata.getHeadColumnOffset(metadata->getIndex());
	long long indexValue = *(long long *)(rowData + indexOffset);
	LongIndex index = LongIndex(indexValue, this->index->getSize());

	this->index->add(index, rowData);
}

double Table::sumDouble(int columnIndex) {
	int columnOffset = metadata->getRowMetadata().getHeadColumnOffset(columnIndex);

	double result = 0;
	if (index->begin()) {
		do {
			double columnValue = *(double*)(index->getData() + columnOffset);
			result += columnValue;
		} while (index->next());
	}

	return result;
}

double Table::sumInt(int columnIndex) {
	int columnOffset = metadata->getRowMetadata().getHeadColumnOffset(columnIndex);

	long long sum = 0;
	if (index->begin()) {
		do {
			int columnValue = *(int*)(index->getData() + columnOffset);
			sum += columnValue;
		} while (index->next());
	}

	return (double)sum;
}

double Table::sum(int columnIndex) {
	if (!metadata) {
		throw exception("Table not initialized yet");
	}

	ColumnType columnType = metadata->getColumns()[columnIndex].getType();

	switch (columnType) {
	case INT:
		return sumInt(columnIndex);
	case DOUBLE:
		return sumDouble(columnIndex);
	default:
		throw exception("Sum is not supported for the column");
	}
}

int Table::count() {
	int result = 0;
	if (index->begin()) {
		do {
			result++;
		} while (index->next());
	}

	return result;
}

}