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
	static string executable_path();
	static string config_path();
	static string data_path();
	static vector<string> template_paths();
	static string local_config_path();
	static string system_config_path();
	static string local_data_path();
	static string system_data_path();
	static string local_templates_path();
	static string system_templates_path();
};