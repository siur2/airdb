#include "RecordValue.h"

#include <algorithm>

namespace AirDB {

using namespace std;

RecordValue::RecordValue() {
	value = 0;
}

RecordValue::~RecordValue() {
	free(value);
}

void RecordValue::validate(ColumnType type) {
	if (this->value == 0) {
		throw exception("Tried to access null value");
	}

	if (this->type != type) {
		throw exception("Value does not match requested type");
	}
}

ColumnType RecordValue::getType() {
	return type;
}

bool RecordValue::isNull() {
	return value == 0;
}

void RecordValue::setInt(int value) {
	free(this->value);
	this->type = INT;
	this->value = malloc(sizeof(int));
	*(int*)(this->value) = value;
}

int RecordValue::getInt() {
	validate(INT);
	return *(int*)value;
}

void RecordValue::setLong(long long value) {
	free(this->value);
	this->type = LONG;
	this->value = malloc(sizeof(long long));
	*(long long*)(this->value) = value;
}

long long RecordValue::getLong() {
	validate(LONG);
	return *(long long*)value;
}

void RecordValue::setDouble(double value) {
	free(this->value);
	this->type = DOUBLE;
	this->value = malloc(sizeof(double));
	*(double*)(this->value) = value;
}

double RecordValue::getDouble() {
	validate(DOUBLE);
	return *(double*)value;
}

void RecordValue::setCStr(const char* value) {
	free(this->value);
	this->type = STRING;
	int length = (int)strlen(value);
	this->value = malloc(length + 1);
	memcpy(this->value, value, length + 1);
}

const char* RecordValue::getCStr() {
	validate(STRING);
	return (char*)value;
}

}