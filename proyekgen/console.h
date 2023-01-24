#pragma once
#include "global.h"

#define print Console()
#define print_debug Console(MessagePriority::Debug)
#define print_warning Console(MessagePriority::Warning)
#define print_error Console(MessagePriority::Error)
#define newline Console::end()

using std::pair;
using string = std::string;

enum class MessagePriority
{
	Normal = 0, Debug = 1, Warning = 2, Error = 3
};

class Console
{
public:
	Console(MessagePriority priority);
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

	MessagePriority _priority;
};
