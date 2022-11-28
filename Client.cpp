#include "Client.h"
#include <sstream>
#include <iterator>

using namespace std;

Client::Client() :
	kSequenceSize(64 / (8 * sizeof(unsigned char)))
{
}

void Client::ProcessCommand(string& command)
{
	const size_t position = command.find_first_of('\r');
	command = command.substr(0, position);
	std::istringstream input_stream(command);
	std::vector<std::string> words(std::istream_iterator<std::string>{input_stream},
		std::istream_iterator<std::string>());
	constexpr int kSize = 2;
	if (words.size() < kSize) {
		return;
	}
	if (words[0] == "seq1" || words[0] == "seq2" || words[0] == "seq3") {
		if (!sequences_[words[0]].empty()) {
			sequences_[words[0]].clear();
		}
		if (words.size() < (kSize + 1)) {
			return;
		}
		sequences_[words[0]] = GenerateSequence(words[1], words[2]);
	}
	else if (words[0] == "export" && words[1] == "seq") {
		message_ = GenerateMessage();
	}
}

vector<unsigned char> Client::GenerateSequence(const string& first_value, const string& step)
{
	vector<unsigned char> sequence{};

	int first_value_number;
	int step_number;
	try {
		first_value_number = stoi(first_value);
		step_number = stoi(step);
	}
	catch (exception& ex) {
		return sequence;
	}

	if (first_value_number <= 0 || step_number <= 0) {
		return sequence;
	}

	sequence.reserve(kSequenceSize);

	constexpr int max_value = 255;
	for (int i = 0; i < kSequenceSize; ++i) {
		if (first_value_number > max_value) {
			first_value_number = stoi(first_value);
		}
		sequence.push_back(first_value_number);
		first_value_number += step_number;
	}
	return sequence;
}

string Client::GenerateMessage()
{
	string message{""};
	vector<string> keys{ "seq1", "seq2", "seq3" };
	bool is_empty = false;
	for (size_t i = 0; i < kSequenceSize; ++i) {
		for (size_t j = 0; j < keys.size(); ++j) {
			is_empty = false;
			if (sequences_.find(keys[j]) == sequences_.end()) {
				is_empty = true;
				continue;
			}
			message += to_string(sequences_[keys[j]][i]) + " ";
		}
		if (!is_empty) {
			message += "\n";
		}
	}
	return message;
}

string Client::GetMessage() const
{
	return message_;
}
