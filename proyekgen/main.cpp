#include "console.h"
#include "error.h"
#include "system.h"
#include "template.h"

// console.h
bool _verbose;

int main(int argc, char *argv[])
{
	// Setup terminate handler
	SystemRuntime::catch_termination();

	// Declare and init command-line argument option variables (e.g help, version, verbose)
	cxxopts::Options args("proyekgen", "A simple and easy project generator.");

	args.add_options("Main")
		("t,template", "Specify template name (or path)", cxxopts::value<string>()->default_value(string()))
		("o,output", "Specify output directory", cxxopts::value<string>()->default_value(SystemPaths::current_path()));
	args.add_options("Other")
		("h,help", "View help information")
		("v,version", "Print program version")
		("verbose", "Enable verbose logging", cxxopts::value<bool>()->default_value("false"));

	args.allow_unrecognised_options();
	args.parse_positional({"template"});
	cxxopts::ParseResult result = args.parse(argc, argv);

	_verbose = result["verbose"].as<bool>();

	// Print out command-line arguments in verbose
	if (result.arguments().size() > 0) {
		print_verbose << "Command-line arguments:" << newline;

		for (const cxxopts::KeyValue &arg : result.arguments()) {
			print_verbose << "	" << arg.key() << ": " << arg.value() << newline;
		}
	}
	if (result.unmatched().size() > 0) {
		print_verbose << "Ignored command-line arguments:" << newline;

		for (const string &arg : result.unmatched()) {
			print_verbose << "	" << arg << newline;
		}
	}

	// Do specific code when some argument options were passed
	if (result.count("help")) {
		cxxopts::Options help = args.custom_help("<TEMPLATE> [OPTIONS...]");
		print << help.positional_help(string()).help() << newline;
		return EXIT_SUCCESS;
	}
	if (result.count("version")) {
		print << "1.0.0" << newline;
		return EXIT_SUCCESS;
	}

	// Generate a project using the given template from the command-line arguments
	TemplateLibrary library;
	string template_name = (result.count("template")) ? result["template"].as<string>() : string();
	string output_path = result["output"].as<string>();

	while (template_name.empty()) {
		template_name = input("Specify template name (or path): ");
	}

	// Parse existing template and generate it's project data into the output path
	Template _template = library.get(template_name); // Screw you "template" keyword
	steady_clock::time_point start = steady_clock::now();
	print_verbose << "Template:" << newline;
	print_verbose << "	name: " << _template.info().name() << newline;
	print_verbose << "	author: " << _template.info().author() << newline;
	print_verbose << "	full path: " << _template.info().path() << newline;
	print_verbose << "Output:" << newline;
	print_verbose << "	" << output_path << newline;
	print_verbose << newline;

	if (!filesystem::is_directory(output_path)) {
		print_verbose << "Output path does not exist, making a new one..." << newline;
		filesystem::create_directories(output_path);
	}

	_template.project().extract(output_path);

	// Print elapsed time at the end of process
	steady_clock::time_point end = steady_clock::now();
	long long elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
	print << "Finished at " << elapsed << "ms." << newline;
	return 0;
}
