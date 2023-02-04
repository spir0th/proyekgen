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
 * A class that carries the template's project data.
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
 * The template class.
 * 
 * Used for accessing information and extracting the template project.
*/
class Template
{
public:
	Template(TemplateInfo info, TemplateProject project);
	Template();

	TemplateInfo info();
	TemplateProject project();
	void set_info(TemplateInfo info);
	void set_project(TemplateProject project);

private:
	TemplateInfo _info;
	TemplateProject _project;
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
	vector<string> search_paths = SystemPaths::template_paths();
};
