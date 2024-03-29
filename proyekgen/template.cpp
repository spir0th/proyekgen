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

#include "template.h"

TemplateProject::TemplateProject(const file_path& path)
	: _path(path)
{}

TemplateProject::TemplateProject()
{}

/*
 * Returns the path of the template's project file.
*/
file_path TemplateProject::path()
{
	return _path;
}

/*
 * Set the path of the template's project file.
*/
void TemplateProject::set_path(const file_path& path)
{
	_path = path;
}

/*
 * Extract the template project to a specified destination
 * 
 * This function returns the result of file extraction, and sometimes
 * may cause a fatal error if the file does not exist / is not a file /
 * is inaccessible.
*/
bool TemplateProject::extract(const string &dest)
{
	struct archive *reader;
	struct archive *writer;
	struct archive_entry *entry;
	file_path cwd = SystemPaths::current_path();
	int result;
	int flags;

	flags = ARCHIVE_EXTRACT_TIME;
	flags |= ARCHIVE_EXTRACT_PERM;
	flags |= ARCHIVE_EXTRACT_ACL;
	flags |= ARCHIVE_EXTRACT_FFLAGS;

	chdir(dest.c_str());

	reader = archive_read_new();
	archive_read_support_format_tar(reader);
	archive_read_support_filter_xz(reader);
	writer = archive_write_disk_new();
	archive_write_disk_set_options(writer, flags);
	archive_write_disk_set_standard_lookup(writer);
	result = archive_read_open_filename(reader, _path.string().c_str(), 10240);

	if (result != ARCHIVE_OK) {
		fmt::print("Failed to read template data: {0:s}", _path);
		SystemRuntime::fatal();
	}
	for (;;) {
		result = archive_read_next_header(reader, &entry);

		if (result == ARCHIVE_EOF) {
			break;
		}
		if (result < ARCHIVE_OK) {
			fmt::print("{0:s}\n", archive_error_string(reader));
			return false;
		}
		if (result < ARCHIVE_WARN) {
			return false;
		}

		result = archive_write_header(writer, entry);
		fmt::print("Writing file: {0:s}\n", archive_entry_pathname(entry));

		if (result < ARCHIVE_OK) {
			fmt::print("{0:s}\n", archive_error_string(writer));
			return false;
		} else if (archive_entry_size(entry) > 0) {
			result = copy(reader, writer);

			if (result < ARCHIVE_OK) {
				fmt::print("{0:s}\n", archive_error_string(writer));
				return false;
			}
			if (result < ARCHIVE_WARN) {
				return false;
			}
		}

		result = archive_write_finish_entry(writer);

		if (result < ARCHIVE_OK) {
			fmt::print("{0:s}\n", archive_error_string(writer));
			return false;
		}
		if (result < ARCHIVE_WARN) {
			return false;
		}
	}

	archive_read_free(reader);
	archive_write_free(writer);
	chdir(cwd.string().c_str());
	return true;
}

/*
 * Internally used by the extract function
*/
int TemplateProject::copy(struct archive *r, struct archive *w)
{
	const void *buffer;
	la_int64_t offset;
	size_t size;
	int result;

	for (;;) {
		result = archive_read_data_block(r, &buffer, &size, &offset);

		if (result == ARCHIVE_EOF) {
			return ARCHIVE_OK;
		}
		if (result < ARCHIVE_OK) {
			return result;
		}

		result = archive_write_data_block(w, buffer, size, offset);

		if (result < ARCHIVE_OK) {
			fmt::print("{0:s}\n", archive_error_string(w));
			return result;
		}
	}
}

TemplateRunner::TemplateRunner(const file_path & path)
	: _path(path)
{
	init();
}

TemplateRunner::TemplateRunner()
{
	init();
}

file_path TemplateRunner::path()
{
	return _path;
}

void TemplateRunner::set_path(const file_path &path)
{
	_path = path;
}

void TemplateRunner::execute()
{
	if (!filesystem::is_regular_file(_path)) {
		fmt::print("{0:s} is not a valid Lua script.", _path);
		SystemRuntime::fatal();
	}

	luaL_dofile(_lua, _path.string().c_str());
	lua_getglobal(_lua, "_pgen_main");
	int result = lua_pcall(_lua, 0, 0, 0);

	if (result != 0) {
		fmt::print("An error occurred while running script: {0:s}\n", lua_tostring(_lua, -1));
	}
}

void TemplateRunner::init()
{
	_lua = luaL_newstate();

	if (_lua == nullptr) {
		fmt::print("Cannot initialize Lua.");
		SystemRuntime::fatal();
	}
	
	luaL_openlibs(_lua);
}

TemplateBase::TemplateBase(TemplateProject project, vector<TemplateRunner> runners)
	: _project(project), _runners(runners)
{}

TemplateBase::TemplateBase()
{}

/*
 * Returns the template's project
*/
TemplateProject TemplateBase::project()
{
	return _project;
}

/*
 * Returns the template's runners
*/
vector<TemplateRunner> TemplateBase::runners()
{
	return _runners;
}

/*
 * Sets the template's project.
*/
void TemplateBase::set_project(TemplateProject project)
{
	_project = project;
}

/*
 * Sets the template's runners;
*/
void TemplateBase::set_runners(vector<TemplateRunner> runners)
{
	_runners = runners;
}

Template::Template(TemplateProject project, vector<TemplateRunner> runners,
	const string &name, const string &author, file_path path)
	: TemplateBase(project, runners), _name(name), _author(author), _path(path) 
{}

Template::Template()
{}

/*
 * Returns the template identifier
 *
 * An identifier is used for searching specific templates in proyekgen.
 * The identifier is extracted from the directory name of the template data.
*/
string Template::identifier()
{
	return _path.filename().string();
}

/*
 * Returns the template name
*/
string Template::name()
{
	return _name;
}

/*
 * Returns the template author
*/
string Template::author()
{
	return _author;
}

/*
 * Returns the fully-qualified path of the template
*/
file_path Template::path()
{
	return _path;
}

/*
 * Set the template's name
*/
void Template::set_name(const string& name)
{
	_name = name;
}

/*
 * Set the template's author
*/
void Template::set_author(const string& author)
{
	_author = author;
}

TemplateLibrary::TemplateLibrary(const vector<string> &paths)
{
	// Add additional search paths passed from the constructor arguments
	search_paths.insert(search_paths.end(), make_move_iterator(paths.begin()),
		make_move_iterator(paths.end()));

	// Initialize for searching templates
	init();
}

TemplateLibrary::TemplateLibrary()
{
	// Initialize for searching templates without additional search paths
	init();
}

/*
 * Get the list of templates installed.
*/
vector<Template> TemplateLibrary::list()
{
	return templates;
}

/*
 * Get the template using the specified name
 * 
 * This function may cause the program to lead
 * in a fatal error if the template doesn't exist.
*/
Template TemplateLibrary::get(const string &name)
{
	if (!exists(name)) {
		fmt::print("Cannot find template with the matching name: {0:s}\n", name);
		SystemRuntime::fatal();
	}
	for (Template t : templates) {
		if (t.identifier() != name) {
			continue;
		}

		return t;
	}

	return Template();
}

/*
 * Remove template from the library
 * 
 * Unused function, this might be used later in the future.
 * 
 * This function may cause the program to lead
 * in a fatal error if the template doesn't exist.
*/
bool TemplateLibrary::remove(string name)
{
	if (!exists(name)) {
		fmt::print("Cannot find template with the matching name: {0:s}\n", name);
		SystemRuntime::fatal();
	}
	
	Template t = get(name);
	std::uintmax_t result = filesystem::remove_all(t.path());
	return (result > 0) ? true : false;
}

/*
 * Returns true if template exists.
*/
bool TemplateLibrary::exists(const string &name)
{
	for (Template t : templates) {
		if (t.identifier() != name) {
			continue;
		}

		return true;
	}

	return false;
}

/*
 * Initialize library.
 * 
 * This function searches for templates and stores them into a vector container.
*/
void TemplateLibrary::init()
{
	for (const file_path &path : search_paths) {
		if (!filesystem::is_directory(path)) {
			continue;
		}
		for (const dir_entry& entry : filesystem::directory_iterator{ path }) {
			string path = entry.path().string();
			string path_filename = entry.path().filename().string();
			file_path project_path = path + separator + "project.tar.xz";
			file_path info_path = path + separator + "info.json";

			if (!filesystem::is_regular_file(project_path) || !filesystem::is_regular_file(info_path)) {
				continue;
			}

			// Info
			json info_json = json::object();
			file_input info_stream(info_path);
			info_json = json::parse(info_stream);

			string name = (info_json.contains("name")) ? static_cast<string>(info_json["name"]) : path_filename;
			string author = (info_json.contains("author")) ? static_cast<string>(info_json["author"]) : "unknown";

			// Project Data
			TemplateProject project = TemplateProject(project_path);

			// Runners
			json runners_json = (info_json.contains("runners")) ? info_json["runners"] : json::array();
			vector<TemplateRunner> runners;

			for (auto &r : runners_json) {
				file_path runner = r;

				if (runner.is_relative()) {
					runner = path + separator + runner.string();
				}

				runners.push_back(TemplateRunner(runner));
			}

			// Add template to vector container
			templates.push_back(Template(project, runners, name, author, path));
		}
	}
}
