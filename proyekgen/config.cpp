#include "config.h"

string ConfigPaths::generator()
{
	return SystemPaths::config_path() + path_separator + "generator.json";
}

string ConfigPaths::global()
{
	return SystemPaths::config_path() + path_separator + "globals.json";
}

string ConfigPaths::templates()
{
	return SystemPaths::config_path() + path_separator + "templates.json";
}

string ConfigPaths::version()
{
	return SystemPaths::config_path() + path_separator + "version.json";
}

json Config::generator()
{
	json cfg = json::object();
	string path = ConfigPaths::generator();

	if (!filesystem::is_regular_file(path)) {
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		rethrow_exception(make_exception_ptr(ex));
	}

	return cfg;
}

json Config::global()
{
	json cfg = json::object();
	string path = ConfigPaths::global();

	if (!filesystem::is_regular_file(path)) {
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		rethrow_exception(make_exception_ptr(ex));
	}

	return cfg;
}

json Config::templates()
{
	json cfg = json::object();
	string path = ConfigPaths::templates();

	if (!filesystem::is_regular_file(path)) {
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		rethrow_exception(make_exception_ptr(ex));
	}

	return cfg;
}

json Config::version()
{
	json cfg = json::object();
	string path = ConfigPaths::version();

	if (!filesystem::is_regular_file(path)) {
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		rethrow_exception(make_exception_ptr(ex));
	}

	return cfg;
}