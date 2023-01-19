#pragma once
#include <iostream>
#include <string>

#if defined(_WIN32)
#include <Windows.h>
#endif

#define newline Console::end();

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
	Console& operator<<(const string& str);
	Console& operator<<(int num);
	Console& operator<<(long num);
	Console& operator<<(double num);

	void log(const char *ch);
	void log(const string &str);
	void log(int num);
	void log(long num);
	void log(double num);

	static string end();

private:
	void init();
	void paint();
	void end_paint();

	MessagePriority _priority;
	string tag;
};