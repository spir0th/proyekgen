#pragma once
#include <filesystem>
#include <fstream>
#include <string>

#include "nlohmann/json.hpp"

using filepath = std::filesystem::path;
using file_input = std::ifstream;
using file_output = std::ofstream;
using string = std::string;
using json = nlohmann::json;

/*
A base class that reads and stores config information.
*/
class Config
{
public:
	Config(string path);

private:
	json data;
};