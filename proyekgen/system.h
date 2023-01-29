#pragma once
#include "global.h"
#include "logger.h"

using std::back_inserter;
using std::count_if;
using std::to_string;
using std::transform;

class SystemRuntime
{
public:
	static bool is_admin_or_root();
	static void fatal(int code = EXIT_FAILURE);
};

class SystemBasePaths
{
	static log4cxx::LoggerPtr _logger;

public:
	static string global_config_path();
	static string global_data_path();
	static string global_templates_path();
	static string local_config_path();
	static string local_data_path();
	static string local_templates_path();
};

class SystemPaths
{
public:
	static string executable_path();
	static string current_path();
	static vector<string> config_paths();
	static vector<string> data_paths();
	static vector<string> template_paths();
};