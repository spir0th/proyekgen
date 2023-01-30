#include "system.h"
#include "template.h"

int main(int argc, char *argv[])
{
	// Init logging using the glog library
	FLAGS_logtostdout = 1;
	FLAGS_colorlogtostdout = 1;
	google::InitGoogleLogging(argv[0]);
	google::InstallFailureSignalHandler();

	// Init application configuration
	for (const string &config_path : SystemPaths::config_paths()) {
		const string &config_file = config_path + separator + "init.cfg";
		Config config;

		if (!filesystem::is_regular_file(config_file)) {
			continue;
		}
		try {
			config.readFile(config_file.c_str());
		} catch (ConfigParseException ex) {
			LOG(WARNING) << "Cannot read configuration file: " << ex.getFile()
				<< ", at line " << ex.getLine();
			continue;
		}
	}

	// Parse command-line arguments
	Options options = Options("proyekgen", string());

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
		("debug", "Enable verbose logging", cxxopts::value<bool>()->default_value("false"));
	options.add_options("Misc")
		("h,help", "View help information")
		("v,version", "Print program version");

	options.allow_unrecognised_options();
	options.parse_positional({"template"});
	OptionsResult options_result = options.parse(argc, argv);

	// Print out command-line arguments if debug
	if (options_result.arguments().size() > 0) {
		VLOG(1) << "Command-line arguments:";

		for (const cxxopts::KeyValue &arg : options_result.arguments()) {
			VLOG(1) << "	" << arg.key() << ": " << arg.value();
		}
	}
	if (options_result.unmatched().size() > 0) {
		VLOG(1) << "Ignored command-line arguments:";

		for (const string &arg : options_result.unmatched()) {
			VLOG(1) << "	" << arg;
		}
	}

	// Do specific code when some argument options were passed
	if (options_result.count("help")) {
		Options help = options.custom_help("<TEMPLATE> [OPTIONS...]");
		LOG(INFO) << help.positional_help(string()).help();
		return EXIT_SUCCESS;
	}
	if (options_result.count("version")) {
		LOG(INFO) << "1.0.0";
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
		LOG(FATAL) << "Implement shell class to ask template name";
	}

	// Parse existing template and generate it's project data into the output path
	Template _template = library.get(template_name);
	steady_clock::time_point start = steady_clock::now();
	LOG(INFO) << "Template:";
	LOG(INFO) << "	name: " + _template.info().name();
	LOG(INFO) << "	author: " + _template.info().author();
	LOG(INFO) << "	full path: " + _template.info().path();
	LOG(INFO) << "Output:";
	LOG(INFO) << "	" + output_path;

	if (!filesystem::is_directory(output_path)) {
		if (!options_result["mkdir"].as<bool>()) {
			LOG(FATAL) << "Output directory doesn't exist,"
				<< " append \"--mkdir\" to automatically create one.";
		}

		LOG(INFO) << "Output directory doesn't exist, making a new one.";
		filesystem::create_directories(output_path);
	}

	_template.project().extract(output_path);

	// Log elapsed time at the end of process
	steady_clock::time_point end = steady_clock::now();
	long long elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	LOG(INFO) << "Finished at " << elapsed << "ms.";
	return 0;
}
