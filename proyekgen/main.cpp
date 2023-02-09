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

#include "input.h"
#include "system.h"
#include "template.h"

int main(int argc, char *argv[])
{
	// Read application configuration from a list of paths
	for (const file_path &config_path : SystemPaths::config_paths()) {
		const string &config_filepath = config_path.string() + separator + "init.cfg";
		config config_file;

		if (!filesystem::is_regular_file(config_filepath)) {
			continue;
		}
		try {
			config_file.readFile(config_filepath.c_str());
		} catch (libconfig::ParseException ex) {
			fmt::print("Cannot read configuration file {0:s} at {1:s}\n", ex.getFile(), ex.getLine());
			continue;
		}
	}

	// Parse command-line arguments
	cmd_options options_parser = cmd_options(PROYEKGEN_HELP_NAME, string());

	options_parser.add_options("Template")
		("t,template", "Specify template name (or path)",
			cxxopts::value<string>()->default_value(string()))
		("s,search-paths", "Append additional search paths",
			cxxopts::value<vector<string>>()->default_value({}), "paths")
		("l,list", "List installed templates")
		("info", "Print template information")
		("skip-generator", "Do not generate the project")
		("skip-runners", "Do not execute runners");
	options_parser.add_options("Output")
		("o,output", "Specify output directory",
			cxxopts::value<string>()->default_value(SystemPaths::current_path().string()), "path");
	options_parser.add_options("Misc")
		("h,help", "View help information")
		("v,version", "Print program version");

	options_parser.parse_positional({"template"});
	auto options = options_parser.parse(argc, argv);

	// Show help info or print program version if passed from command-line options
	if (options.count("help")) {
		const string help = options_parser.custom_help("<TEMPLATE> [OPTIONS...]")
			.positional_help(string()).help();

		fmt::print("{0:s}\n", help);
		return EXIT_SUCCESS;
	}
	if (options.count("version")) {
		fmt::print("{0:s}\n", PROYEKGEN_HELP_VERSION);
		return EXIT_SUCCESS;
	}

	// Find the given template from the command-line options
	vector<string> template_search_paths = options["search-paths"].as<vector<string>>();
	string template_name = options["template"].as<string>();
	file_path output_path = options["output"].as<string>();
	TemplateLibrary library = TemplateLibrary(template_search_paths);

	// Make absolute path for output directory if relative
	if (output_path.is_relative()) {
		output_path = SystemPaths::current_path().string() + separator + output_path.string();
	}
	// List installed templates if passed from command-line options
	if (options.count("list")) {
		vector<Template> templates = library.list();

		if (templates.size() > 0) {
			fmt::print("There are {0:d} templates installed:\n", templates.size());
		} else {
			fmt::print("There are no templates installed.\n", templates.size());
			fmt::print("To install a template, visit the link: %s\n", string());
		}
		for (Template t : templates) {
			TemplateInfo info = t.info();
			string filename = info.path().filename().string();
			string name = "(" + info.name() + ")";

			if (info.name().empty()) {
				name.clear();
			}

			fmt::print("	{0:s} {1:s}\n", filename, name);
		}

		return EXIT_SUCCESS;
	}

	// Always ask for template name (or path) if empty
	while (template_name.empty()) {
		template_name = input("Specify template name (or path): ");
	}

	// Parse template by getting it from the library using it's pathname
	Template _template = library.get(template_name);
	
	// Show template information only if "--info" is passed from command-line options
	if (options.count("info")) {
		fmt::print("Template:\n");
		fmt::print("	name: {0:s}\n", _template.info().name());
		fmt::print("	author: {0:s}\n", _template.info().author());
		fmt::print("	path: {0:s}\n", _template.info().path());

		if (!_template.runners().empty()) {
			fmt::print("	runners:\n");
		}
		for (TemplateRunner runner : _template.runners()) {
			fmt::print("		{0:}\n", runner.path().filename());
		}

		return EXIT_SUCCESS;
	}
	// Generate and execute runners if "--skip-generate" isn't passed from command-line options
	if (!options.count("skip-generator")) {
		if (!filesystem::is_directory(output_path)) {
			// Create directories if output directory is non-existent
			fmt::print("Creating directory: {0:s}\n", output_path.stem());
			filesystem::create_directories(output_path);
		}
		if (!_template.project().extract(output_path.string())) {
			// Generate project using given template and extract the project data
			fmt::print("Generate failure while extracting project data.\n");
		}
	}
	// Execute each runners if "--skip-runners" isn't passed from command-line options
	if (!options.count("skip-runners")) {
		for (TemplateRunner runner : _template.runners()) {
			// Temporarily change directory to output path
			const file_path& cwd = SystemPaths::current_path();
			chdir(output_path.string().c_str());
			
			// Execute runner, change directory back to current path when done
			runner.execute();
			chdir(cwd.string().c_str());
		}
	}

	return 0;
}
