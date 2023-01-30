#include "template.h"

TemplateInfo::TemplateInfo(string name, string author, string path)
	: _name(name), _author(author), _path(path), _platform(TemplatePlatform::Any)
{}

/*
Return the template name
*/
string TemplateInfo::name()
{
	return _name;
}

/*
Return the template author
*/
string TemplateInfo::author()
{
	return _author;
}

/*
Return the fully-qualified path of the template
*/
string TemplateInfo::path()
{
	return _path;
}

log4cxx::LoggerPtr TemplateProject::_logger(get_logger("TemplateProject"));

TemplateProject::TemplateProject(string path)
	: _path(path)
{
	if (!filesystem::is_regular_file(_path)) {
		LOG4CXX_FATAL(_logger, _path << " does not exist/or is not an archive.");
		SystemRuntime::fatal();
	}
}

/*
Extract the template project to a specified destination

This function returns the result of file extraction
*/
bool TemplateProject::extract(const string &dest)
{
	struct archive *reader;
	struct archive *writer;
	struct archive_entry *entry;
	string cwd = SystemPaths::current_path();
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
	result = archive_read_open_filename(reader, _path.c_str(), 10240);

	if (result != ARCHIVE_OK) {
		LOG4CXX_FATAL(_logger, "Failed to read archive file: " << _path);
		SystemRuntime::fatal(result);
	}
	for (;;) {
		result = archive_read_next_header(reader, &entry);

		if (result == ARCHIVE_EOF) {
			break;
		}
		if (result < ARCHIVE_OK) {
			LOG4CXX_ERROR(_logger, archive_error_string(reader));
			return false;
		}
		if (result < ARCHIVE_WARN) {
			return false;
		}

		result = archive_write_header(writer, entry);
		LOG4CXX_DEBUG(_logger, "Extracting: " << archive_entry_pathname(entry));

		if (result < ARCHIVE_OK) {
			LOG4CXX_ERROR(_logger, archive_error_string(writer));
			return false;
		} else if (archive_entry_size(entry) > 0) {
			result = copy(reader, writer);

			if (result < ARCHIVE_OK) {
				LOG4CXX_ERROR(_logger, archive_error_string(writer));
				return false;
			}
			if (result < ARCHIVE_WARN) {
				return false;
			}
		}

		result = archive_write_finish_entry(writer);

		if (result < ARCHIVE_OK) {
			LOG4CXX_ERROR(_logger, archive_error_string(writer));
			return false;
		}
		if (result < ARCHIVE_WARN) {
			return false;
		}
	}

	archive_read_free(reader);
	archive_write_free(writer);
	chdir(cwd.c_str());
	return true;
}

/*
Internally used by the extract function
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
			LOG4CXX_ERROR(_logger, archive_error_string(w));
			return result;
		}
	}
}

Template::Template(TemplateInfo info, TemplateProject project)
	: _info(info), _project(project) {}

/*
Return the template information
*/
TemplateInfo Template::info()
{
	return _info;
}

/*
Return the template project
*/
TemplateProject Template::project()
{
	return _project;
}

log4cxx::LoggerPtr TemplateLibrary::_logger(get_logger("TemplateLibrary"));

TemplateLibrary::TemplateLibrary(const vector<string> &paths)
{
	// Add default search paths from SystemPaths
	const vector<string> &default_paths = SystemPaths::template_paths();
	search_paths.insert(search_paths.begin(), make_move_iterator(default_paths.begin()),
		make_move_iterator(default_paths.end()));

	// Also add search paths passed from the constructor arguments
	search_paths.insert(search_paths.end(), make_move_iterator(paths.begin()),
		make_move_iterator(paths.end()));
}

TemplateLibrary::TemplateLibrary()
{
	// Only add default search paths from SystemPaths
	vector<string> default_paths = SystemPaths::template_paths();
	search_paths.insert(search_paths.begin(), make_move_iterator(default_paths.begin()),
		make_move_iterator(default_paths.end()));
}

/*
Get the list of templates in the library
*/
vector<string> TemplateLibrary::list()
{
	vector<string> contents;

	for (string path : search_paths) {
		if (!filesystem::is_directory(path)) {
			continue;
		}
		for (const dir_entry &entry : filesystem::directory_iterator{path}) {
			string project = entry.path().string() + separator + "project.tar.xz";
			string info = entry.path().string() + separator + "info.json";

			if (!filesystem::is_regular_file(project) || !filesystem::is_regular_file(info)) {
					continue;
			}

			contents.emplace_back(entry.path().string());
		}
	}

	return contents;
}

/*
Get the template using the specified name

This function may cause the program to lead
in a fatal error if the template doesn't exist.
*/
Template TemplateLibrary::get(const string &name)
{
	if (!exists(name)) {
		LOG4CXX_FATAL(_logger, "Cannot find template with the matching name: " << name);
		SystemRuntime::fatal();
	}

	string full_path = get_path(name);
	string project_path = full_path + separator + "project.tar.xz";
	string info_path = full_path + separator + "info.json";

	// info.json
	json info_json = json::object();
	file_input info_stream(info_path);
	info_json = json::parse(info_stream);

	string info_name = (info_json.contains("name")) ? (string)info_json["name"] : name;
	string info_author = info_json["author"];

	TemplateInfo info = TemplateInfo(info_name, info_author, full_path);

	// project.tar.xz
	TemplateProject project = TemplateProject(project_path);

	// Final output
	return Template(info, project);
}

/*
Remove template from the library

Unused function, this might be used later in the future.

This function may cause the program to lead
in a fatal error if the template doesn't exist.
*/
bool TemplateLibrary::remove(string name)
{
	if (!exists(name)) {
		LOG4CXX_FATAL(_logger, "Cannot find template with the matching name: " << name);
		SystemRuntime::fatal();
	}
	if (file_path(name).is_relative()) {
		for (string t : list()) {
			string project = t + separator + "project.tar.xz";
			string info = t + separator + "info.json";

			if (!filesystem::is_directory(t)) {
				continue;
			}
			if (!filesystem::is_directory(t) ||
				!filesystem::is_regular_file(project) ||
				!filesystem::is_regular_file(info)) {
				continue;
			}

			name = t + separator + name;
		}
	}

	std::uintmax_t result = filesystem::remove_all(file_path(name));
	return (result > 0) ? true : false;
}

/*
Returns true if template exists in the library
*/
bool TemplateLibrary::exists(const string &name)
{
	string path = get_path(name);
	string project = path + separator + "project.tar.xz";
	string info = path + separator + "info.json";

	if (filesystem::is_directory(path) &&
		filesystem::is_regular_file(project) &&
		filesystem::is_regular_file(info)) {
		return true;
	}

	return false;
}

/*
Returns the fully-qualified path of a template
*/
string TemplateLibrary::get_path(const string &name)
{
	string path = name;

	if (file_path(path).is_relative()) {
		for (string t : list()) {
			string project = t + separator + "project.tar.xz";
			string info = t + separator + "info.json";

			if (!filesystem::is_directory(t) ||
				!filesystem::is_regular_file(project) ||
				!filesystem::is_regular_file(info)) {
				continue;
			}
			if (file_path(t).filename().string() != path) {
				continue;
			}

			path = t;
		}
	}

	return path;
}
