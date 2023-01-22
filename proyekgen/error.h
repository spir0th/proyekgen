#pragma once
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

using runtime_error = std::runtime_error;
using filesystem_error = std::filesystem::filesystem_error;
using string = std::string;

class UnixNoPathPreservedError : public filesystem_error
{
public:
	UnixNoPathPreservedError(const string &what);
};

class Win32InternalError : public runtime_error
{
public:
	Win32InternalError(const string &what);
};
