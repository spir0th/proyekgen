#include "logger.h"
#include "system.h"
#include "template.h"

int main(int argc, char *argv[])
{
	// Init log4cxx and application configuration
	log4cxx::LoggerPtr logger = get_logger("main");

	for (const string &config_path : SystemPaths::config_paths()) {
		const string &config_file = config_path + separator + "init.cfg";
		libconfig::Config config;

		if (!filesystem::is_regular_file(config_file)) {
			continue;
		}
		try {
			config.readFile(config_file);
		} catch (libconfig::ParseException ex) {
			LOG4CXX_WARN(logger, "Cannot read configuration file: " << ex.getFile()
				<< ", at line " << ex.getLine());
			continue;
		}
	}

	// Parse command-line arguments and set some variables if options are passed
	cxxopts::Options args("proyekgen", string());

	args.add_options("Template")
		("t,template", "Specify template name (or path)",
			cxxopts::value<string>()->default_value(string()))
		("search-paths", "Append additional search paths",
			cxxopts::value<vector<string>>()->default_value({}));
	args.add_options("Output")
		("o,output", "Specify output directory",
			cxxopts::value<string>()->default_value(SystemPaths::current_path()))
		("mkdir", "Make output directory (can be recursive) if non-existent",
			cxxopts::value<bool>()->default_value("false"));
	args.add_options("Logging")
		("verbose", "Enable verbose logging", cxxopts::value<bool>()->default_value("false"));
	args.add_options("Misc")
		("h,help", "View help information")
		("v,version", "Print program version");

	args.allow_unrecognised_options();
	args.parse_positional({"template"});
	cxxopts::ParseResult result = args.parse(argc, argv);

	// Print out command-line arguments in verbose
	if (result.arguments().size() > 0) {
		LOG4CXX_DEBUG(logger, "Command-line arguments:");

		for (const cxxopts::KeyValue &arg : result.arguments()) {
			LOG4CXX_DEBUG(logger, "	" << arg.key() << ": " << arg.value());
		}
	}
	if (result.unmatched().size() > 0) {
		LOG4CXX_DEBUG(logger, "Ignored command-line arguments:");

		for (const string &arg : result.unmatched()) {
			LOG4CXX_DEBUG(logger, "	" << arg);
		}
	}

	// Do specific code when some argument options were passed
	if (result.count("help")) {
		cxxopts::Options help = args.custom_help("<TEMPLATE> [OPTIONS...]");
		LOG4CXX_INFO(logger, help.positional_help(string()).help());
		return EXIT_SUCCESS;
	}
	if (result.count("version")) {
		LOG4CXX_INFO(logger, "1.0.0");
		return EXIT_SUCCESS;
	}

	// Generate a project using the given template from the command-line arguments
	TemplateLibrary library = TemplateLibrary(result["search-paths"].as<vector<string>>());
	string template_name = (result.count("template")) ? result["template"].as<string>() : string();
	string output_path = result["output"].as<string>();

	while (template_name.empty()) {
		// TODO: Implement shell class to ask input
		// template_name = input("Specify template name (or path): ");
		SystemRuntime::fatal(logger, "Implement shell class to ask template name");
	}

	// Parse existing template and generate it's project data into the output path
	Template _template = library.get(template_name); // Screw you "template" keyword
	steady_clock::time_point start = steady_clock::now();
	LOG4CXX_DEBUG(logger, "Template:");
	LOG4CXX_DEBUG(logger, "	name: " + _template.info().name());
	LOG4CXX_DEBUG(logger, "	author: " + _template.info().author());
	LOG4CXX_DEBUG(logger, "	full path: " + _template.info().path());
	LOG4CXX_DEBUG(logger, "Output:");
	LOG4CXX_DEBUG(logger, "	" + output_path);

	if (!filesystem::is_directory(output_path)) {
		if (!result["mkdir"].as<bool>()) {
			SystemRuntime::fatal(logger, "Output directory doesn't exist, append \"--mkdir\" to automatically create one.");
		}

		LOG4CXX_INFO(logger, "Output directory doesn't exist, making a new one.");
		filesystem::create_directories(output_path);
	}

	_template.project().extract(output_path);

	// Print elapsed time at the end of process
	steady_clock::time_point end = steady_clock::now();
	long long elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	LOG4CXX_INFO(logger, "Finished at " + std::to_string(elapsed) + "milisecs.");
	return 0;
}
