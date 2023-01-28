#pragma once
#include "console.h"
#include "error.h"
#include "global.h"

using std::back_inserter;
using std::count_if;
using std::current_exception;
using std::rethrow_exception;
using std::set_terminate;
using std::to_string;
using std::transform;

class SystemRuntime
{
public:
	static void catch_termination();
	static bool is_admin_or_root();
};

class SystemPaths
{
public:
	static string executable_path();
	static string current_path();
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