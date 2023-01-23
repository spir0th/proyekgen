#include "console.h"
#include "system.h"
#include "template.h"

int main(int argc, char *argv[])
{
	print << "Begin console test:" << newline;
	print << "	This is a normal message" << newline;
	print_debug << "	This is a debug message, the \"--debug\" command-line option will reveal the messages that have this priority." << newline;
	print_warning << "	This is a warning message, the \"--no-warn\" command-line option will suppress these warnings." << newline;
	print_error << "	This is an error message, the \"--skip-errors\" command-line option will make the program continue running even there are errors." << newline;
	print << "End console test..." << newline;
	print << "Begin log paths:" << newline;
	print << "	executable path: " << SystemPaths::executable_path() << newline;
	print << "	config path: " << SystemPaths::config_path() << newline;
	print << "	data path: " << SystemPaths::data_path() << newline;
	print << "	local config path: " << SystemPaths::local_config_path() << newline;
	print << "	local data path: " << SystemPaths::local_data_path() << newline;
	print << "	system config path: " << SystemPaths::system_config_path() << newline;
	print << "	system data path: " << SystemPaths::system_data_path() << newline;
	print << "End log paths..." << newline;
	print << "Does have administrative/root privileges: ";

	if (SystemRuntime::is_admin_or_root()) {
		print << "yes" << newline;
	} else {
		print << "no" << newline;
	}

	print << "Testing template info:" << newline;
	Template test_template = TemplateLibrary().get("test");
	print << "	name: " << test_template.info().name() << newline;
	print << "	author: " << test_template.info().author() << newline;
	return 0;
}
