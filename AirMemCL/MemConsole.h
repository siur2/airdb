#pragma once

#include "Table.h"
#include "RBTree.h"

namespace AirMemCL {

using namespace AirDB;

class MemConsole {
private:
	Table* table;
	string fileName;

	void splitString(vector<string>& result, const string str, char delim);
	void test();
	void test2();
	void printTree(RBTree& tree);
	static bool testComparatorLT(void* data1, void* data2);
	static bool testComparatorEQ(void* data1, void* data2);
public:
	MemConsole();
	~MemConsole();
	void run();
	void readTable(const vector<string>& args);
	void saveTable();
	void sum(const vector<string>& args);
	void total();
	void count();
	void showMetadata();
};

}

