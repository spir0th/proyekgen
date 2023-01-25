#pragma once
#include "global.h"

#define print Console()
#define print_debug Console(ConsolePriority::Debug)
#define print_warning Console(ConsolePriority::Warning)
#define print_error Console(ConsolePriority::Error)
#define newline Console::end()

using std::cin;
using std::cout;
using std::clog;
using std::getline;

enum class ConsolePriority
{
	Normal = 0, Debug = 1, Warning = 2, Error = 3
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
	Console& operator<<(double num);
	Console& operator>>(pair<const string&, string> args);
	Console& operator>>(pair<const string&, int> args);

	void log(const char *ch);
	void log(const string &str);
	void log(int num);
	void log(long num);
	void log(double num);
	void input(const string &msg, string out);
	void input(const string &msg, int out);

	static string end();

private:
	void paint();
	void end_paint();

	ConsolePriority _priority;
};
