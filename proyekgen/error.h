#pragma once
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>

using runtime_error = std::runtime_error;
using filesystem_error = std::filesystem::filesystem_error;
using string = std::string;

class LibArchiveInternalError : public runtime_error
{
public:
	LibArchiveInternalError(const string &what);
};

class ProjectFileNotFoundError : public filesystem_error
{
public:
	ProjectFileNotFoundError(const string &what);
};

class TemplateNotFoundError : public filesystem_error
{
public:
	TemplateNotFoundError(const string &what);
};

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
