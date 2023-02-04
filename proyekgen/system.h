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
	static file_path global_config_path();
	static file_path global_data_path();
	static file_path global_templates_path();
	static file_path local_config_path();
	static file_path local_data_path();
	static file_path local_templates_path();
};

/*
 * An utility class that provides path-reserved functions.
*/
class SystemPaths
{
public:
	static file_path executable_path();
	static file_path current_path();
	static vector<file_path> config_paths();
	static vector<file_path> data_paths();
	static vector<file_path> template_paths();
};
