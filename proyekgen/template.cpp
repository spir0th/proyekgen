#include "template.h"

TemplateInfo::TemplateInfo(const string &name, const string &author, file_path path)
	: _name(name), _author(author), _path(path)
{}

TemplateInfo::TemplateInfo()
{}

/*
 * Returns the template name
*/
string TemplateInfo::name()
{
	return _name;
}

/*
 * Returns the template author
*/
string TemplateInfo::author()
{
	return _author;
}

/*
 * Returns the fully-qualified path of the template
*/
file_path TemplateInfo::path()
{
	return _path;
}

/*
 * Set the template's name
*/
void TemplateInfo::set_name(const string &name)
{
	_name = name;
}

/*
 * Set the template's author
*/
void TemplateInfo::set_author(const string &author)
{
	_author = author;
}

TemplateProject::TemplateProject(const file_path &path)
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
void TemplateProject::set_path(const file_path &path)
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
	chdir(cwd.c_str());
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

Template::Template(TemplateInfo info, TemplateProject project)
	: _info(info), _project(project)
{}

Template::Template()
{}

/*
 * Returns the template's information
*/
TemplateInfo Template::info()
{
	return _info;
}

/*
 * Returns the template's project
*/
TemplateProject Template::project()
{
	return _project;
}

/*
 * Sets the template's info.
*/
void Template::set_info(TemplateInfo info)
{
	_info = info;
}

/*
 * Sets the template's project.
*/
void Template::set_project(TemplateProject project)
{
	_project = project;
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
		if (t.info().path().filename().string() != name) {
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
	std::uintmax_t result = filesystem::remove_all(t.info().path());
	return (result > 0) ? true : false;
}

/*
 * Returns true if template exists.
*/
bool TemplateLibrary::exists(const string &name)
{
	for (Template t : templates) {
		if (t.info().path().filename().string() != name) {
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
	for (string path : search_paths) {
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

			// info.json
			json info_json = json::object();
			file_input info_stream(info_path);
			info_json = json::parse(info_stream);

			string info_name = (info_json.contains("name")) ? static_cast<string>(info_json["name"]) : path_filename;
			string info_author = (info_json.contains("author")) ? static_cast<string>(info_json["author"]) : "unknown";

			TemplateInfo info = TemplateInfo(info_name, info_author, path);

			// project.tar.xz
			TemplateProject project = TemplateProject(project_path);

			// Add template to vector container
			templates.push_back(Template(info, project));
		}
	}
}
