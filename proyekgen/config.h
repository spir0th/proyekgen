#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "console.h"
#include "error.h"
#include "nlohmann/json.hpp"
#include "system.h"

namespace filesystem = std::filesystem;

using filepath = std::filesystem::path;
using file_input = std::ifstream;
using file_output = std::ofstream;
using string = std::string;
using json = nlohmann::json;

/*
A helper class to locate config files.
*/
class ConfigPaths
{
public:
	static string generator();
	static string global();
	static string templates();
	static string version();
};

/*
The application config class, it is used in most classes to utilize settings.
*/
class Config
{
public:
	static json generator();
	static json global();
	static json templates();
	static json version();
};