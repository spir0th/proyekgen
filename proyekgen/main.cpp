#include "logger.h"
#include "system.h"
#include "template.h"

int main(int argc, char *argv[])
{
	// Initialize application logging powered by spdlog
	spdlog::set_pattern("[%l] %v");
	spdlog::set_error_handler([](const string& msg) {
		using std::clog, std::endl;
		clog << "Logging error occurred: " << msg << endl;
	});

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
			LOG_WARN("Cannot read configuration file {0} at line {1}", ex.getFile(), ex.getLine());
			continue;
		}
	}

	// Parse command-line arguments
	cmd_options options = cmd_options("proyekgen", string());

	options.add_options("Template")
		("t,template", "Specify template name (or path)",
			cxxopts::value<string>()->default_value(string()))
		("search-paths", "Append additional search paths",
			cxxopts::value<vector<string>>()->default_value({}));
	options.add_options("Output")
		("o,output", "Specify output directory",
			cxxopts::value<string>()->default_value(SystemPaths::current_path()))
		("mkdir", "Make output directory (can be recursive) if non-existent",
			cxxopts::value<bool>()->default_value("false"));
	options.add_options("Log")
		("debug", "Enable debug logging", cxxopts::value<bool>()->default_value("false"));
	options.add_options("Misc")
		("h,help", "View help information")
		("version", "Print program version");

	options.allow_unrecognised_options();
	options.parse_positional({"template"});
	auto options_result = options.parse(argc, argv);

	// Set level to debug if --debug option is passed
	if (options_result["debug"].as<bool>()) {
		spdlog::set_level(spdlog::level::debug);
	}

	// Log command-line arguments in debugging sink
	if (options_result.arguments().size() > 0) {
		LOG_DEBUG("Command-line arguments:");

		for (const cxxopts::KeyValue &arg : options_result.arguments()) {
			LOG_DEBUG("	{0}: {1}", arg.key(),  arg.value());
		}
	}
	if (options_result.unmatched().size() > 0) {
		LOG_DEBUG("Ignored command-line arguments:");

		for (const string &arg : options_result.unmatched()) {
			LOG_DEBUG("	{0}", arg);
		}
	}

	// Help and Version options passing code
	if (options_result.count("help")) {
		cmd_options help = options.custom_help("<TEMPLATE> [OPTIONS...]");
		LOG_INFO_RAW(help.positional_help(string()).help());
		return EXIT_SUCCESS;
	}
	if (options_result.count("version")) {
		LOG_INFO_RAW("1.0.0");
		return EXIT_SUCCESS;
	}

	// Generate a project using the given template from the command-line arguments
	vector<string> template_search_paths = options_result["search-paths"].as<vector<string>>();
	string template_name = options_result["template"].as<string>();
	string output_path = options_result["output"].as<string>();
	TemplateLibrary library = TemplateLibrary(template_search_paths);

	while (template_name.empty()) {
		// TODO: Implement shell class to ask input
		// template_name = input("Specify template name (or path): ");
		LOG_CRITICAL("Implement shell class to ask template name", 1);
	}

	// Parse existing template and generate it's project data into the output path
	Template _template = library.get(template_name);
	steady_clock::time_point start = steady_clock::now();
	LOG_INFO("Template:");
	LOG_INFO("	name: {0}", _template.info().name());
	LOG_INFO("	author: {0}", _template.info().author());
	LOG_INFO("	full path: {0}", _template.info().path());
	LOG_INFO("Output:");
	LOG_INFO("	{0}", output_path);

	if (!filesystem::is_directory(output_path)) {
		if (!options_result["mkdir"].as<bool>()) {
			LOG_CRITICAL("Output directory doesn't exist,  append \"--mkdir\" to automatically create one.", 2);
		}

		LOG_INFO("Output directory doesn't exist, making a new one.");
		filesystem::create_directories(output_path);
	}
	if (!_template.project().extract(output_path)) {
		LOG_CRITICAL("Generation failure while extracting project data.", 3);
	}

	// Log elapsed time at the end of process
	steady_clock::time_point end = steady_clock::now();
	long long elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	LOG_INFO("Finished at {} msecs.", elapsed);
	return 0;
}
