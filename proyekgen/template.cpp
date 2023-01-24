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

TemplateProject::ExtractResult TemplateProject::extract()
{
	struct archive *reader;
	struct archive *writer;
	struct archive_entry *entry;
	int result;
	int flags;

	flags = ARCHIVE_EXTRACT_TIME;
	flags |= ARCHIVE_EXTRACT_PERM;
	flags |= ARCHIVE_EXTRACT_ACL;
	flags |= ARCHIVE_EXTRACT_FFLAGS;

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
			return TemplateProject::ExtractResult::Bad;
		}

		result = archive_write_header(writer, entry);

		if (result < ARCHIVE_OK) {
			print_error << archive_error_string(writer) << newline;
		} else if (archive_entry_size(entry) > 0) {
			result = copy(reader, writer);

			if (result < ARCHIVE_OK) {
				print_error << archive_error_string(writer) << newline;
			}
			if (result < ARCHIVE_WARN) {
				return TemplateProject::ExtractResult::Bad;
			}
		}

		result = archive_write_finish_entry(writer);

		if (result < ARCHIVE_OK) {
			print_error << archive_error_string(writer) << newline;
		}
		if (result < ARCHIVE_WARN) {
			return TemplateProject::ExtractResult::Bad;
		}
	}

	archive_read_free(reader);
	archive_write_free(writer);
	return TemplateProject::ExtractResult::Good;
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

TemplateLibrary::TemplateLibrary()
	: config(Config::templates())
{
	// Concatenate extra search paths put from templates.json
	vector<string> extra_paths = config.value("paths", vector<string>());
	search_paths.insert(search_paths.end(), make_move_iterator(extra_paths.begin()),
		make_move_iterator(extra_paths.end()));
}

vector<string> TemplateLibrary::list()
{
	vector<string> contents;

	for (string path : search_paths) {
		if (!filesystem::is_directory(path)) {
			continue;
		}
		for (const dir_entry &entry : filesystem::directory_iterator{path}) {
			string project = entry.path().string() + "/project.tar.xz";
			string info = entry.path().string() + "/info.json";

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
	string project_path = full_path + "/project.tar.xz";
	string info_path = full_path + "/info.json";

	// info.json
	json info_json = json::object();
	file_input info_stream(info_path);
	info_json = json::parse(info_stream);

	string info_name = filepath(full_path).filename().string();
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
	if (filepath(name).is_relative()) {
		for (string t : list()) {
			string project = t + "/project.tar.xz";
			string info = t + "/info.json";

			if (!filesystem::is_directory(t)) {
				continue;
			}
			if (!filesystem::is_directory(t) ||
				!filesystem::is_regular_file(project) ||
				!filesystem::is_regular_file(info)) {
				continue;
			}

			name = t + "/" + name;
		}
	}

	std::uintmax_t result = filesystem::remove_all(filepath(name));
	return (result > 0) ? true : false;
}

bool TemplateLibrary::exists(const string &name)
{
	string path = get_path(name);
	string project = path + "/project.tar.xz";
	string info = path + "/info.json";

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

	if (filepath(path).is_relative()) {
		for (string t : list()) {
			string project = t + "/project.tar.xz";
			string info = t + "/info.json";

			if (!filesystem::is_directory(t) ||
				!filesystem::is_regular_file(project) ||
				!filesystem::is_regular_file(info)) {
				continue;
			}
			if (filepath(t).filename().string() != path) {
				continue;
			}

			path = t;
		}
	}

	return path;
}