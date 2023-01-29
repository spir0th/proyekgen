#pragma once
#include <algorithm>
#include <chrono>
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
#elif defined(__unix__) or defined(__MACH__)
#include "limits.h"
#include "unistd.h"
#endif

#include "archive.h"
#include "archive_entry.h"
#include "cxxopts.hpp"
#include "nlohmann/json.hpp"

#define separator (char)std::filesystem::path::preferred_separator

namespace chrono = std::chrono;
namespace filesystem = std::filesystem;

using dir_entry = std::filesystem::directory_entry;
using exception = std::exception;
using exception_ptr = std::exception_ptr;
using file_path = std::filesystem::path;
using file_input = std::ifstream;
using file_output = std::ofstream;
using json = nlohmann::json;
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