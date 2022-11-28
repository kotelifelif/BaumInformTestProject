#pragma once
#include <string>
#include <map>
#include <vector>

using namespace std;

class Client
{
public:
	Client();
	void ProcessCommand(string& command);
	vector<unsigned char> GenerateSequence(const string& first_value, const string& step);
	string GenerateMessage();
	string GetMessage() const;
private:
	map<string, vector<unsigned char>> sequences_;
	const size_t kSequenceSize;
	string message_;
};

