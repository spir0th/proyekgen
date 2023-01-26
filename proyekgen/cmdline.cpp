#include "cmdline.h"

CommandLineArguments::CommandLineArguments(const vector<string>& data)
	: _data(data)
{}

CommandLineArguments::CommandLineArguments()
	: _data({})
{}

string CommandLineArguments::get_string(const string &option)
{
	vector<string>::const_iterator iterator = find(_data.begin(), _data.end(), option);
	
	if (iterator != _data.end() && iterator++ != _data.end()) {
		return *iterator;
	}

	return string();
}

int CommandLineArguments::get_int(const string &option)
{
	return stoi(get_string(option));
}

vector<string> CommandLineArguments::data()
{
	return _data;
}

bool CommandLineArguments::exists(const string &option)
{
	return find(_data.begin(), _data.end(), option) != _data.end();
}

CommandLineArguments _parse_cmd_args(int argc, char* argv[])
{
	vector<string> args;

	if (argc > 1) {
		args.assign(argv + 1, argv + argc);
	}

	return CommandLineArguments(args);
}