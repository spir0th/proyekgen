#include "input.h"
#include "system.h"
#include "template.h"

int main(int argc, char *argv[])
{
	// Read application configuration from a list of paths
	for (const string &config_path : SystemPaths::config_paths()) {
		const string &config_filepath = config_path + separator + "init.cfg";
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
		("l,list", "List installed templates");
	options_parser.add_options("Output")
		("o,output", "Specify output directory",
			cxxopts::value<string>()->default_value(SystemPaths::current_path()))
		("m,mkdir", "Make output directory if it does not exist",
			cxxopts::value<bool>()->default_value("false"));
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
			// TODO: Print a help link that leads to templates.
		}
		for (Template t : templates) {
			TemplateInfo info = t.info();
			file_path filename = info.path().filename();
			string name = "(" + info.name() + ")";

			if (info.name().empty()) {
				name.clear();
			}

			fmt::print("	{0:s} {1:s}\n", filename.string(), name);
		}

		return EXIT_SUCCESS;
	}

	while (template_name.empty()) {
		template_name = input("Specify template name (or path): ");
	}

	Template _template = library.get(template_name);

	// Generate project using given template and it's project data
	steady_clock::time_point timer_start = steady_clock::now();
	fmt::print("Template:\n");
	fmt::print("	name: {0:s}\n", _template.info().name());
	fmt::print("	author: {0:s}\n", _template.info().author());
	fmt::print("	path: {0:s}\n", _template.info().path());
	fmt::print("Output:\n");
	fmt::print("	{0:s}\n\n", output_path);

	if (!filesystem::is_directory(output_path)) {
		if (!options["mkdir"].as<bool>()) {
			fmt::print("Output directory doesn't exist, append \"--mkdir\" to automatically create one.\n");
			SystemRuntime::fatal();
		}

		fmt::print("Output directory doesn't exist, making a new one.\n");
		filesystem::create_directories(output_path);
	}
	if (!_template.project().extract(output_path)) {
		fmt::print("Generation failure while extracting project data.\n");
	}

	// Log elapsed time at the end of process
	steady_clock::time_point timer_end = steady_clock::now();
	chrono::duration<double> timer_diff = timer_end - timer_start;
	fmt::print("Finished at {0} milliseconds.\n", timer_diff);
	return 0;
}
