#pragma once

#include <vector>

namespace AirFileCL {

using namespace std;

class FileConsole {
private:
	void splitString(vector<string>& result, const string str, char delim);
	void test();
	void test2();
	static bool testCompare(void* first, void* second);
public:
	FileConsole();
	~FileConsole();
	void run();
};

}