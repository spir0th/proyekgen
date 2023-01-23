#pragma once
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "error.h"

#if defined(_WIN32)
#include <Windows.h>
#include <ShlObj.h>
#elif defined(__unix__) or defined(__MACH__)
#include "limits.h"
#include "unistd.h"
#endif

namespace filesystem = std::filesystem;

using std::back_inserter;
using string = std::string;
using stringstream = std::stringstream;
using std::vector;
using std::transform;
using wstring = std::wstring;
using wstringstream = std::wstringstream;

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