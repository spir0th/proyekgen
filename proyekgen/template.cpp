#include "template.h"

TemplateInfo::TemplateInfo(string name, string author, string path)
	: _name(name), _author(author), _path(path), _platform(TemplatePlatform::Any)
{}

string TemplateInfo::name()
{
	return _name;
}

string TemplateInfo::author()
{
	return _author;
}

string TemplateInfo::path()
{
	return _path;
}

TemplateProject::TemplateProject(string path)
	: _path(path)
{
	if (!filesystem::is_regular_file(_path)) {
		throw ProjectFileNotFoundError(_path + " does not exist/or is not an archive.");
	}
}

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
		throw LibArchiveInternalError("Failed to read archive file: " + _path);
	}
	for (;;) {
		result = archive_read_next_header(reader, &entry);

		if (result == ARCHIVE_EOF) {
			break;
		}
		if (result < ARCHIVE_OK) {
			print_error << archive_error_string(reader) << newline;
		}
		if (result < ARCHIVE_WARN) {
			return false;
		}

		result = archive_write_header(writer, entry);
		print_verbose << "Extracting: " << archive_entry_pathname(entry) << newline;

		if (result < ARCHIVE_OK) {
			print_error << archive_error_string(writer) << newline;
		} else if (archive_entry_size(entry) > 0) {
			result = copy(reader, writer);

			if (result < ARCHIVE_OK) {
				print_error << archive_error_string(writer) << newline;
			}
			if (result < ARCHIVE_WARN) {
				return false;
			}
		}

		result = archive_write_finish_entry(writer);

		if (result < ARCHIVE_OK) {
			print_error << archive_error_string(writer) << newline;
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
			print_error << archive_error_string(w) << newline;
			return result;
		}
	}
}

Template::Template(TemplateInfo info, TemplateProject project)
	: _info(info), _project(project) {}

TemplateInfo Template::info()
{
	return _info;
}

TemplateProject Template::project()
{
	return _project;
}

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

Template TemplateLibrary::get(const string &name)
{
	if (!exists(name)) {
		throw TemplateNotFoundError("Cannot find template with the matching name: " + name);
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

bool TemplateLibrary::remove(string name)
{
	if (!exists(name)) {
		throw TemplateNotFoundError("Cannot find template with the matching name: " + name);
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