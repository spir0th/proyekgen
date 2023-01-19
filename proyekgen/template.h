#pragma once
#include <filesystem>
#include <iostream>
#include <string>

// #include "zstr.hpp"

using filepath = std::filesystem::path;
using string = std::string;

enum class TemplatePlatform
{
	Any = 0, Windows = 1, Linux = 2, macOS = 3
};

/*
A class that holds the template's information.
*/
class TemplateInfo
{
public:
	TemplateInfo(string name, string author, TemplatePlatform *platform = nullptr);

private:
	string *_name = nullptr;
	string *_author = nullptr;
	TemplatePlatform _platform = TemplatePlatform::Any;
};

/*
A class that holds the template's project data.
*/
class TemplateProject
{
public:
	TemplateProject();
};

/*
The class that holds everything about the template.
*/
class Template
{
public:
	Template(TemplateInfo info, TemplateProject project);

	TemplateInfo info();
	TemplateProject project();

private:
	TemplateInfo *_info = nullptr;
	TemplateProject *_project = nullptr;
};

/*
A helper class that manages templates easily.
*/
class TemplateLibrary
{

};