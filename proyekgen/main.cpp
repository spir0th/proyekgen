#include "console.h"
#include "error.h"
#include "system.h"
#include "template.h"

// console.h
bool _verbose;

// template.h
CommandLineArguments _cmd_args;

int main(int argc, char *argv[])
{
	// Initialize command-line arguments, and setup terminate handler
	SystemRuntime::init(argc, argv);
	SystemRuntime::catch_termination();

	// Declare and init command-line argument option variables (e.g help, version, verbose)
	CommandLineArguments args = SystemRuntime::args();
	bool is_help = args.exists("-h") || args.exists("--help");
	bool is_version = args.exists("-v") || args.exists("--version");
	_verbose = args.exists("--verbose");

	// Print out command-line arguments if verbose is enabled
	print_verbose << "Command-line arguments:" << newline;

	for (const string &arg : args.data()) {
		print_verbose << "	" << arg << newline;
	}

	// Generate a project using the given template on the command-line argument
	vector<string> args_raw = args.data();
	TemplateLibrary library;
	string template_name;
	string output_path;

	// Additional command-line argument parsing (for template name/path and output path)
	if (args_raw.size() > 0) {
		template_name = args_raw[0];
	} else {
		while (template_name.empty()) {
			template_name = input("Specify template name or path: ");
		}
	}
	if (args.exists("-o")) {
		output_path = args.get_string("-o");
	} else if (args.exists("--output")) {
		output_path = args.get_string("--output");
	} else {
		output_path = SystemPaths::current_path();
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
