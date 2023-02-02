#pragma once
#include "global.h"

using std::back_inserter;
using std::count_if;
using std::to_string;
using std::transform;

/*
 * An utility class that provides runtime functions.
*/
class SystemRuntime
{
public:
	static bool is_admin_or_root();
	static void fatal(int code = EXIT_FAILURE);
};

/*
 * Internally used by the SystemPaths class.
 * 
 * Accessing functions from this class might not be recommended
 * as they provide separate functions for non-root/root privileges.
*/
class SystemBasePaths
{
public:
	static string global_config_path();
	static string global_data_path();
	static string global_templates_path();
	static string local_config_path();
	static string local_data_path();
	static string local_templates_path();
};

/*
 * An utility class that provides path-reserved functions.
*/
class SystemPaths
{
public:
	static string executable_path();
	static string current_path();
	static vector<string> config_paths();
	static vector<string> data_paths();
	static vector<string> template_paths();
};
