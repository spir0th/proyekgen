#pragma once
#include "global.h"
#include "system.h"

#define input Console().ask
#define print Console()
#define print_verbose Console(ConsolePriority::Verbose)
#define print_warning Console(ConsolePriority::Warning)
#define print_error Console(ConsolePriority::Error)
#define newline Console::end()

using std::cin;
using std::cout;
using std::clog;
using std::getline;

extern bool _verbose;

enum class ConsolePriority
{
	Normal = 0, Verbose = 1, Warning = 2, Error = 3
};

class Console
{
public:
	Console(ConsolePriority priority);
	Console();

	Console& operator<<(const char *ch);
	Console& operator<<(const string &str);
	Console& operator<<(int num);
	Console& operator<<(long num);
	Console& operator<<(long long num);
	Console& operator<<(double num);

	void log(const char *ch);
	void log(const string &str);
	void log(int num);
	void log(long num);
	void log(long long num);
	void log(double num);
	string ask(const string &msg);

	static string end();

private:
	void paint();
	void end_paint();

	ConsolePriority _priority;
};
