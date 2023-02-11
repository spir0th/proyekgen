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
#pragma warning(disable: 4244)
#pragma warning(disable: 4275)
#include <algorithm>
#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#if defined(_WIN32)
#include <Windows.h>
#include <ShlObj.h>
#include <direct.h>
#define chdir _chdir
#elif defined(__linux__)
#include "limits.h"
#include "unistd.h"
#elif defined(__APPLE__) && defined(__MACH__)
#error Building on macOS is not supported.
#endif

#include "archive.h"
#include "archive_entry.h"
#include "cxxopts.hpp"
#include "fmt/color.h"
#include "fmt/core.h"
#include "fmt/ranges.h"
#include "fmt/std.h"
#include "libconfig.h++"
#include "lua.hpp"
#include "nlohmann/json.hpp"

#define separator (char)std::filesystem::path::preferred_separator

namespace filesystem = std::filesystem;

using config = libconfig::Config;
using dir_entry = std::filesystem::directory_entry;
using exception = std::exception;
using exception_ptr = std::exception_ptr;
using file_path = std::filesystem::path;
using file_input = std::ifstream;
using file_output = std::ofstream;
using json = nlohmann::json;
using cmd_options = cxxopts::Options;
template<class Key, class T>
using map = std::map<Key, std::less<Key>, std::allocator<std::pair<const Key, T>>>;
template<class Key, class T>
using pair = std::pair<Key, T>;
using steady_clock = std::chrono::steady_clock;
using string = std::string;
using stringstream = std::stringstream;
template<class T>
using vector = std::vector<T, std::allocator<T>>;
using wstring = std::wstring;
using wstringstream = std::wstringstream;