/*
	proyekgen - A simple project generator
	Copyright (C) 2023 spirothXYZ

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#pragma once
#include "global.h"

using std::back_inserter;
using std::count_if;
using std::getline;
using std::to_string;
using std::transform;

/*
 * An utility class that provides runtime functions.
*/
class SystemRuntime
{
public:
	static bool is_root();
	static string input(const string &msg);
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
