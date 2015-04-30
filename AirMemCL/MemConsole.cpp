#include "stdafx.h"
#include "MemConsole.h"

#include <iostream>
#include <ctime>
#include <sstream>

namespace AirMemCL {

using namespace std;

MemConsole::MemConsole() {
	table = 0;
}

MemConsole::~MemConsole() {
	delete table;
}

void MemConsole::run() {
	cout << fixed;
	vector<string> args;
	while (true) {
		cout << "air>";
		string line;
		getline(cin, line);
		splitString(args, line, ' ');

		string msg = args.front();

		try {
			if (msg == "exit") {
				cout << "Bye";
				break;
			}
			else
			if (msg == "read") {
				readTable(args);
			}
			else
			if (msg == "save") {
				saveTable();
			}
			else
			if (msg == "sum") {
				sum(args);
			}
			else
			if (msg == "total") {
				total();
			}
			else
			if (msg == "count") {
				count();
			}
			else
			if (msg == "meta") {
				showMetadata();
			}
			else
			if (msg == "test") {
				test();
			}
			else
			if (msg == "test2") {
				test2();
			}
			else {
				cout << "Unknown command - \"" + msg + "\"\n";
			}
		}
		catch (exception& e) {
			cout << e.what() << endl;
		}
	}
}

void MemConsole::readTable(const vector<string>& args) {
	if (args.size() != 2) {
		throw exception("Invalid number of arguments");
	}

	delete table;
	table = new Table();
	fileName = "D:\\Air\\" + args[1] + ".dat";
	int time = table->readFromFile(fileName.c_str());
	cout << "read " << table->getRowCount() << " records in " << time << " msec\n";
}

void MemConsole::saveTable() {
	int time = table->saveToFile(fileName.c_str());
	cout << "saved table in " << time << " msec\n";
}

void MemConsole::sum(const vector<string>& args) {
	if (!table) {
		throw exception("Table not initialized yet");
	}

	if (args.size() != 2) {
		throw exception("Invalid number of arguments");
	}

	int index = stoi(args[1]) - 1;
	clock_t startTime = clock();
	double result = table->sum(index);
	int time = clock() - startTime;
	cout << result << endl;
	cout << "query executed in " << time << " msec\n";
}

void MemConsole::total() {
	if (!table) {
		throw exception("Table not initialized yet");
	}

	cout << table->getRowCount() << endl;
}

void MemConsole::count() {
	if (!table) {
		throw exception("Table not initialized yet");
	}

	clock_t startTime = clock();
	int result = table->count();
	int time = clock() - startTime;
	cout << result << endl;
	cout << "query executed in " << time << " msec\n";
}

void MemConsole::showMetadata() {
	if (!table) {
		throw exception("Table not initialized yet");
	}

	const vector<ColumnMetadata>& columns = table->getMetadata().getColumns();
	for (int i = 0; i < columns.size(); i++) {
		const ColumnMetadata& column = columns[i];
		cout << (i + 1) << "." << column.getName() << " ";
		cout << ColumnMetadata::getColumnTypeName(column.getType()) << endl;
	}
}

void MemConsole::splitString(vector<string>& result, const string str, char delim) {
	result.clear();
	stringstream ss(str);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
}

bool MemConsole::testComparatorLT(void* data1, void* data2) {
	int value1 = *(int*)data1;
	int value2 = *(int*)data2;
	return  value1 < value2;
}

bool MemConsole::testComparatorEQ(void* data1, void* data2) {
	int value1 = *(int*)data1;
	int value2 = *(int*)data2;
	return  value1 == value2;
}

void MemConsole::test() {
	RBTree tree(testComparatorLT, testComparatorEQ, sizeof(int));
	int values[30];
	for (int i = 0; i < _countof(values); i++) {
		values[i] = i + 1;
		tree.add(&values[i]);
	}

	for (int i = 6; i <= 25; i++) {
		if (!tree.remove(&i)) {
			throw exception("Node not found for the key supplied");
		}
	}

	printTree(tree);

	int value;
	cout << endl << "Lets delete first & last:" << endl;
	value = 1;
	tree.remove(&value);
	value = 30;
	tree.remove(&value);
	printTree(tree);

	cout << endl << "Add few values:" << endl;
	for (int i = 14; i < 17; i++) {
		tree.add(&values[i]);
	}
	printTree(tree);

	value = 28;
	int* v28 = (int*)tree.find(&value);
	if (v28 != 0) {
		cout << "found value = " << *v28 << endl;
	}

	cout << endl << "Finally delete all:" << endl;
	RBTree::Iterator it;
	while (tree.begin(&it)) {
		tree.remove(it.getData());
	}
	cout << "tree size: " << tree.getSize() << endl;
}

void MemConsole::test2() {
	RBTree tree(testComparatorLT, testComparatorEQ, sizeof(int));
	int values[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 51};

	for (int i = 0; i < 10; i++) {
		tree.add(&values[i]);
	}

	for (int i = 10; i < 20; i++) {
		tree.add(&values[i]);
	}

	tree.add(&values[20]);

	printTree(tree);
}

void MemConsole::printTree(RBTree& tree) {
	cout << "tree size: " << tree.getSize() << endl;

	RBTree::Iterator it;
	tree.begin(&it);
	do {
		cout << *((int*)it.getData()) << endl;
	} while (it.next());
}

}