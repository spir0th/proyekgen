#pragma once
#include "error.h"
#include "global.h"

using std::back_inserter;
using std::transform;

class SystemRuntime
{
public:
	static bool is_admin_or_root();
};

class SystemPaths
{
public:
	SystemPaths();

	static string executable_path();
	static string config_path();
	static string data_path();
	static vector<string> template_paths();

private:
	string local_config_path();
	string system_config_path();
	string local_data_path();
	string system_data_path();
	string local_templates_path();
	string system_templates_path();
};