#include "stdafx.h"
#include "FileConsole.h"
#include "FileRBTree.h"

#include <iostream>
#include <ctime>
#include <sstream>
#include <memory>

namespace AirFileCL {

using namespace AirDB;

FileConsole::FileConsole() {
}

FileConsole::~FileConsole() {
}

void FileConsole::run() {
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

void FileConsole::splitString(vector<string>& result, const string str, char delim) {
	result.clear();
	stringstream ss(str);
	string item;
	while (getline(ss, item, delim)) {
		result.push_back(item);
	}
}

void FileConsole::test() {
	char fileName[] = "d:\\air\\test.idx";
	unique_ptr<FileRBTree> tree(new FileRBTree(fileName, testCompare));
	for (int i = 0; i < 10; i++) {
		tree->add(i + 1);
	}

	tree.reset();
	tree.reset(new FileRBTree(fileName, testCompare));
	for (int i = 90; i < 100; i++) {
		tree->add(i + 1);
	}

	tree.reset();
	tree.reset(new FileRBTree(fileName, testCompare));
	for (int i = 50; i < 53; i++) {
		tree->add(i + 1);
	}

	tree.reset();
	tree.reset(new FileRBTree(fileName, testCompare));
	if (!tree->validate()) {
		cout << "Invalid tree" << endl;
		return;
	}

	tree.reset();
	tree.reset(new FileRBTree(fileName, testCompare));
	FileRBTree::Iterator it;
	for (tree->first(&it); !it.isEmpty(); it.next()) {
		cout << it.getIndexData() << endl;
	}
	
	tree.reset();
	File::delVerify(fileName);
}

void FileConsole::test2() {
	char fileName[] = "d:\\air\\test2.idx";
	int batchSize = 10000;
	int insertCount = 1000000;
	File::del(fileName);

	clock_t startTime = clock();

	unique_ptr<FileRBTree> tree(new FileRBTree(fileName, testCompare));
	/*for (int i = 0; i < insertCount; i++) {
		if ((i > 0) && (i % batchSize == 0)) {
			tree.reset();
			tree.reset(new FileRBTree(fileName, testCompare));
		}
		tree->add(i + 1);
	}*/
	for (int i = 0; i < insertCount; i++) {
		if ((i > 0) && (i % batchSize == 0)) {
			tree->clearBuffer();
		}
		tree->add(i + 1);
	}

	int time = clock() - startTime;
	cout << "inserted " << insertCount << " in " << time << " msec" << endl;

	startTime = clock();
	tree.reset();
	tree.reset(new FileRBTree(fileName, testCompare));
	cout << "Node count: " << tree->getNodeCount() << endl;
	bool valid = tree->validate();
	time = clock() - startTime;
	cout << "Tree is " << (valid ? "valid" : "invalid") << ". validated tree in " << time << " msec" << endl;

	cout << "Node count: " << tree->getNodeCount() << endl;
	for (int i = 0; i < batchSize; i++) {
		tree->add(i + 1);
	}
	cout << batchSize << " nodes inserted" << endl;
	cout << "Node count: " << tree->getNodeCount() << endl;
}

bool FileConsole::testCompare(void* first, void* second) {
	long long value1 = *(long long*)first;
	long long value2 = *(long long*)second;
	return value1 < value2;
}

}