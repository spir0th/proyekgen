#pragma once
#include "console.h"
#include "global.h"
#include "system.h"

namespace filesystem = std::filesystem;

using std::make_move_iterator;

enum class TemplatePlatform
{
	Any = 0, Windows = 1, Linux = 2, macOS = 3
};

/*
A class that contains the template's information.
*/
class TemplateInfo
{
public:
	TemplateInfo(string name, string author, string path = string());

	string name();
	string author();
	string path();

private:
	string _name;
	string _author;
	string _path;
	TemplatePlatform _platform;
};

/*
A class that carries the template's project data.
*/
class TemplateProject
{
public:
	TemplateProject(string path);

	bool extract(const string &dest);

private:
	int copy(struct archive *r, struct archive *w);

	string _path;
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
	TemplateInfo _info;
	TemplateProject _project;
};

/*
A class that manages templates easily.
*/
class TemplateLibrary
{
public:
	TemplateLibrary(const vector<string> &paths);
	TemplateLibrary();

	vector<string> list();
	Template get(const string &name);
	bool remove(string name);
	bool exists(const string &name);

private:
	string get_path(const string &name);

	vector<string> search_paths = SystemPaths::template_paths();
};