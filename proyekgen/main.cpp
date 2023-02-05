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
			cxxopts::value<vector<string>>()->default_value({}))
		("l,list", "List installed templates")
		("skip-generator", "Do not generate the project")
		("skip-runners", "Do not execute runners after generating");
	options_parser.add_options("Output")
		("o,output", "Specify output directory",
			cxxopts::value<string>()->default_value(SystemPaths::current_path().string()))
		("mkdir", "Make output directory if it does not exist");
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
	string output_path = options["output"].as<string>();
	TemplateLibrary library = TemplateLibrary(template_search_paths);

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

	// Always ask for template name if empty
	while (template_name.empty()) {
		template_name = input("Specify template name (or path): ");
	}

	// Parse template and start elapsed timer
	Template _template = library.get(template_name);
	steady_clock::time_point timer_start = steady_clock::now();

	// Print template info and it's runners before generating
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

	fmt::print("\n"); // Make a newline

	// Generate and execute runners if "skip-*" options weren't passed on the command-line
	if (!options.count("skip-generator")) {
		if (!filesystem::is_directory(output_path)) {
			// Create directories or lead to fatal error if output directory is non-existent
			if (!options.count("mkdir")) {
				fmt::print("Output directory doesn't exist, append \"--mkdir\" to automatically create one.\n");
				SystemRuntime::fatal();
			}

			fmt::print("Output directory doesn't exist, making a new one.\n");
			filesystem::create_directories(output_path);
		}
		if (!_template.project().extract(output_path)) {
			// Generate project using given template and extract the project data
			fmt::print("Generate failure while extracting project data.\n");
		}
	}
	if (!options.count("skip-runners")) {
		for (TemplateRunner runner : _template.runners()) {
			// Run each runners after generating project
			runner.run();
		}
	}

	// Print elapsed time after done
	steady_clock::time_point timer_end = steady_clock::now();
	chrono::duration<double> timer_diff = timer_end - timer_start;
	fmt::print("Finished at {0:%H:%M:%S}.\n", timer_diff);
	return 0;
}
