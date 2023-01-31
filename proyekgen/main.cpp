#include "logger.h"
#include "system.h"
#include "template.h"

void log_error_handler(const string &msg)
{
	using std::clog, std::endl;
	clog << "Logging error occurred: " << msg << endl;
}

int main(int argc, char *argv[])
{
	// Initialize application logging powered by spdlog
	spdlog::set_pattern("%^%v%$");
	spdlog::set_error_handler(log_error_handler);

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
		("s,search-paths", "Append additional search paths",
			cxxopts::value<vector<string>>()->default_value({}))
		("l,list", "List installed templates");
	options.add_options("Output")
		("o,output", "Specify output directory",
			cxxopts::value<string>()->default_value(SystemPaths::current_path()))
		("m,mkdir", "Make output directory if does not exist",
			cxxopts::value<bool>()->default_value("false"));
	options.add_options("Log")
		("d,debug", "Show debug logs", cxxopts::value<bool>()->default_value("false"));
	options.add_options("Misc")
		("h,help", "View help information")
		("v,version", "Print program version");

	options.allow_unrecognised_options();
	options.parse_positional({"template"});
	auto options_result = options.parse(argc, argv);

	// Enable log debugging if passed from command-line options
	if (options_result["debug"].as<bool>()) {
		spdlog::set_level(spdlog::level::debug);
	}

	// Log command-line options in the debug sink
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

	// Show help info or print program version if passed from command-line options
	if (options_result.count("help")) {
		cmd_options help = options.custom_help("<TEMPLATE> [OPTIONS...]");
		LOG_INFO(help.positional_help(string()).help());
		return EXIT_SUCCESS;
	}
	if (options_result.count("version")) {
		LOG_INFO("1.0.0");
		return EXIT_SUCCESS;
	}

	// Find the given template from the command-line options
	vector<string> template_search_paths = options_result["search-paths"].as<vector<string>>();
	string template_name = options_result["template"].as<string>();
	string output_path = options_result["output"].as<string>();
	TemplateLibrary library = TemplateLibrary(template_search_paths);

	// List installed templates if passed from command-line options
	if (options_result.count("list")) {
		vector<string> templates = library.list();

		LOG_INFO("There are {0} template(s) installed{1}", templates.size()
			, (templates.size() > 0) ? ":" : ".");

		for (const string &template_name : templates) {
			Template _template = library.get(template_name);
			TemplateInfo info = _template.info();

			LOG_INFO("	{0} {1}", file_path(info.path()).filename().string(),
				(info.name().length() > 0) ? "(" + info.name() + ")" : string());
		}

		return EXIT_SUCCESS;
	}

	while (template_name.empty()) {
		// TODO: Implement shell class to ask input
		// template_name = input("Specify template name (or path): ");
		LOG_CRITICAL("Implement shell class to ask template name", 1);
	}

	Template _template = library.get(template_name);

	// Generate project using given template and it's project data
	stopwatch elapsed;
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
	LOG_INFO("Finished at {} msecs.", elapsed);
	return 0;
}
