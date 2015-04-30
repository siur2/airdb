#include "ColumnMetadata.h"

#include <exception>

namespace AirDB {

ColumnMetadata::ColumnMetadata() {
}

void ColumnMetadata::init(ColumnType type, const char* name) {
	this->type = type;
	this->name = string(name);
}

ColumnType ColumnMetadata::getType() const {
	return type;
}

const char* ColumnMetadata::getName() const {
	return name.c_str();
}

int ColumnMetadata::getColumnSize() {
	switch (type) {
	case ColumnType::INT:
		return sizeof(int);
	case ColumnType::LONG:
		return sizeof(long long);
	case ColumnType::DOUBLE:
		return sizeof(double);
	case ColumnType::STRING:
		return sizeof(int);
	default:
		throw exception("Unknown column type");
	}
}

string ColumnMetadata::getColumnTypeName(ColumnType type) {
	switch (type) {
	case ColumnType::INT:
		return "int";
	case ColumnType::LONG:
		return "long";
	case ColumnType::DOUBLE:
		return "double";
	case ColumnType::STRING:
		return "string";
	default:
		throw exception("Unknown column type");
	}
}

}