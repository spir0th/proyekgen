#include "config.h"

string ConfigPaths::generator()
{
	return SystemPaths::config_path() + "/generator.json";
}

string ConfigPaths::global()
{
	return SystemPaths::config_path() + "/globals.json";
}

string ConfigPaths::templates()
{
	return SystemPaths::config_path() + "/templates.json";
}

string ConfigPaths::version()
{
	return SystemPaths::config_path() + "/version.json";
}

json Config::generator()
{
	json cfg = json::object();
	string path = ConfigPaths::generator();

	if (!filesystem::is_regular_file(path)) {
		print_warning << "Cannot find generator config file." << newline;
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		print_error << "Generator config parsing failed: " << ex.what() << newline;
	}

	return cfg;
}

json Config::global()
{
	json cfg = json::object();
	string path = ConfigPaths::global();

	if (!filesystem::is_regular_file(path)) {
		print_error << "Cannot find global config file. DAMMIT!" << newline;
		print_warning << "Using default config... This enables features you might not want." << newline;
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		print_error << "Global config parsing failed: " << ex.what() << newline;
	}
	if (cfg == json::object()) {
		// Print warning to make sure the user is notified about using the default config
		print_warning << "Using default config... This enables features you might not want." << newline;
	}

	return cfg;
}

json Config::templates()
{
	json cfg = json::object();
	string path = ConfigPaths::templates();

	if (!filesystem::is_regular_file(path)) {
		print_warning << "Cannot find template config file." << newline;
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		print_error << "Template config parsing failed: " << ex.what() << newline;
	}

	return cfg;
}

json Config::version()
{
	json cfg = json::object();
	string path = ConfigPaths::version();

	if (!filesystem::is_regular_file(path)) {
		print_warning << "Cannot find version config file." << newline;
		return cfg;
	}
	try {
		file_input stream(path);
		cfg = json::parse(stream);
	} catch (json::parse_error ex) {
		print_error << "Version config parsing failed: " << ex.what() << newline;
	}

	return cfg;
}