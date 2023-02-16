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
#include "global.h"
#include "system.h"

using std::make_move_iterator;

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
	void execute();

private:
	void init();

	file_path _path;
	lua_State *_lua = nullptr;
};

/*
 * A base class for templates.
 *
 * To provide template information, use the Template class instead.
*/
class TemplateBase
{
public:
	TemplateBase(TemplateProject project, vector<TemplateRunner> runners);
	TemplateBase();

	TemplateProject project();
	vector<TemplateRunner> runners();
	void set_project(TemplateProject project);
	void set_runners(vector<TemplateRunner> runners);

private:
	TemplateProject _project;
	vector<TemplateRunner> _runners;
};

/*
 * The template class.
*/
class Template : public TemplateBase
{
public:
	Template(TemplateProject project, vector<TemplateRunner> runners,
		const string &name, const string &author, file_path path = string());
	Template();

	string identifier();
	string name();
	string author();
	file_path path();
	void set_name(const string& name);
	void set_author(const string& author);

private:
	string _name;
	string _author;
	file_path _path;
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
	Template get(const string &keyword);
	bool remove(string keyword);
	bool exists(const string &keyword);

private:
	void init();
	
	vector<Template> templates = {};
	vector<file_path> search_paths = SystemPaths::template_paths();
};
