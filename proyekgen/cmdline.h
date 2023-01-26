#pragma once
#include "global.h"

using std::find;
using std::stoi;
using std::tolower;
using std::transform;

class CommandLineArguments
{
public:
	CommandLineArguments(const vector<string> &data);
	CommandLineArguments(); // Used for extern keywords

	string get_string(const string &option);
	int get_int(const string &option);
	vector<string> data();
	bool exists(const string &option);

private:
	vector<string> _data;
};

/* Internally used, call SystemRuntime::args instead */
CommandLineArguments _parse_cmd_args(int argc, char *argv[]);