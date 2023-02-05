#pragma once
#include "global.h"
#include "system.h"

using std::make_move_iterator;

/*
 * A class that contains the template's information.
*/
class TemplateInfo
{
public:
	TemplateInfo(const string &name, const string &author, file_path path = string());
	TemplateInfo();

	string name();
	string author();
	file_path path();
	void set_name(const string &name);
	void set_author(const string &author);

private:
	string _name;
	string _author;
	file_path _path;
};

/*
 * A class that provides the project data of a template.
*/
class TemplateProject
{
public:
	TemplateProject(const file_path &path);
	TemplateProject();

	file_path path();
	void set_path(const file_path &path);
	bool extract(const string &dest);

private:
	int copy(struct archive *r, struct archive *w);

	file_path _path;
};

/*
* A class that runs Lua code before and after generating a project.
*/
class TemplateRunner
{
public:
	TemplateRunner(const file_path &path);
	TemplateRunner();

	file_path path();
	void set_path(const file_path & path);
	void run();

private:
	void init();

	file_path _path;
	lua_State *_lua = nullptr;
};

/*
 * The template class.
 * 
 * Used for accessing information and extracting the template project.
*/
class Template
{
public:
	Template(TemplateInfo info, TemplateProject project, vector<TemplateRunner> runners);
	Template();

	TemplateInfo info();
	TemplateProject project();
	vector<TemplateRunner> runners();
	void set_info(TemplateInfo info);
	void set_project(TemplateProject project);
	void set_runners(vector<TemplateRunner> runners);

private:
	TemplateInfo _info;
	TemplateProject _project;
	vector<TemplateRunner> _runners;
};

/*
 * An utility class that manages templates easily.
*/
class TemplateLibrary
{
public:
	TemplateLibrary(const vector<string> &paths);
	TemplateLibrary();

	vector<Template> list();
	Template get(const string &name);
	bool remove(string name);
	bool exists(const string &name);

private:
	void init();
	
	vector<Template> templates = {};
	vector<file_path> search_paths = SystemPaths::template_paths();
};
