#include "ImportController.h"
#include "TableIO.h"
#include "Record.h"

#include <atlstr.h>

#include <memory>
#include <vector>

namespace AirDBImport {

using namespace System::IO;
using namespace System::Text;
using namespace System::Runtime::Serialization::Json;
using namespace System::Collections::Generic;

[System::Runtime::Serialization::DataContractAttribute]
ref class JsonColumn {
private:
	String^ _name;
	String^ _type;
public:
	[System::Runtime::Serialization::DataMemberAttribute]
	property String^ name { String^ get() { return _name; } void set(String^ value) { _name = value; } }
	[System::Runtime::Serialization::DataMemberAttribute]
	property String^ type { String^ get() { return _type; } void set(String^ value) { _type = value; } }
};

ImportController::ImportController() {
}

ImportController::~ImportController() {
}

long long ImportController::import(String^ connectionString, String^ tableName, String^ tableMetadataJson, int recordsLimit) {
	unique_ptr<TableMetadata> metadata(createMetadata(tableMetadataJson));
	CStringA fileName("D:\\Air\\" + tableName + ".dat");
	TableIO::createTable(fileName, metadata.get());

	SqlConnection^ conn = gcnew SqlConnection(connectionString);
	conn->Open();
	String^ sql = gcnew String("select * from ") + tableName;
	SqlCommand^ cmd = gcnew SqlCommand(sql, conn);
	SqlDataReader^ reader = cmd->ExecuteReader();
	long long result = importRows(fileName, metadata.get(), reader, recordsLimit);
	reader->Close();
	conn->Close();

	return result;
}

long long ImportController::importRows(const char* fileName, TableMetadata* metadata, SqlDataReader^ reader, int recordsLimit) {
	const vector<ColumnMetadata>& columns = metadata->getColumns();
	vector<unique_ptr<Record>> records;
	int batchSize = 10000;
	long long recordsImported = 0;
	while (reader->Read()) {
		Record* record = new Record((int)columns.size());
		records.push_back(unique_ptr<Record>(record));
		vector<RecordValue>* values = record->getValues();

		for (unsigned int i = 0; i < columns.size(); i++) {
			String^ columnName = gcnew String(columns[i].getName());
			ColumnType columnType = columns[i].getType();
			RecordValue* value = &(*values)[i];
			Object^ obj = reader[columnName];
			setRecordValue(columnType, obj, value);
		}

		recordsImported++;

		if (records.size() == batchSize) {
			TableIO::appendRowdata(fileName, &records);
			records.clear();
			System::Console::WriteLine(recordsImported);
		}
		
		if ((recordsLimit > 0) && (recordsImported >= recordsLimit)) {
			break;
		}
	}

	TableIO::appendRowdata(fileName, &records);

	return recordsImported;
}

void ImportController::setRecordValue(ColumnType type, Object^ obj, RecordValue* value) {
	switch (type) {
	case ColumnType::INT:
		if (obj->GetType()->Equals(Int16::typeid)) {
			value->setInt((short)obj);
		} else {
			value->setInt((int)obj);
		}
		break;
	case ColumnType::LONG:
		if (obj->GetType()->Equals(DateTime::typeid)) {
			value->setLong(((DateTime^)obj)->Ticks);
		} else {
			value->setLong((long long)obj);
		}
		break;
	case ColumnType::DOUBLE:
		value->setDouble((double)(*(Decimal^)obj));
		break;
	case ColumnType::STRING:
		if (obj->GetType()->Equals(DBNull::typeid)) {
			value->setCStr("");
		} else {
			CStringA strVal((String^)obj);
			value->setCStr(strVal);
		}
		break;
	}
}

ColumnType ImportController::getColumnType(System::String^ type) {
	if (type == L"long") {
		return ColumnType::LONG;
	}

	if (type == L"int") {
		return ColumnType::INT;
	}

	if (type == L"double") {
		return ColumnType::DOUBLE;
	}

	if (type == L"string") {
		return ColumnType::STRING;
	}

	throw exception("Invalid column type");
}

TableMetadata* ImportController::createMetadata(System::String^ metadataJson) {
	DataContractJsonSerializer^ serializer = gcnew DataContractJsonSerializer(List<JsonColumn^>::typeid);
	MemoryStream^ stream = gcnew MemoryStream(Encoding::UTF8->GetBytes(metadataJson));
	List<JsonColumn^>^ jsonColumns = (List<JsonColumn^>^)serializer->ReadObject(stream);
	unique_ptr<vector<ColumnMetadata>> metadataColumns(new vector<ColumnMetadata>(jsonColumns->Count));

	for (int i = 0; i < jsonColumns->Count; i++) {
		JsonColumn^ column = jsonColumns[i];
		System::Console::WriteLine(column->name + ": " + column->type);
		CStringA columnName(column->name);
		(*metadataColumns)[i].init(getColumnType(column->type), columnName.GetBuffer());
	}

	return new TableMetadata(metadataColumns.release(), 0, 0);
}

}