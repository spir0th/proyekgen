#include "console.h"

Console::Console(ConsolePriority priority)
	: _priority(priority) 
{}

Console::Console()
	: _priority(ConsolePriority::Normal)
{}

Console& Console::operator<<(const char* ch)
{
	log(ch);
	return *this;
}

Console& Console::operator<<(const string &str)
{
	log(str);
	return *this;
}

Console& Console::operator<<(int num)
{
	log(num);
	return *this;
}

Console& Console::operator<<(long num)
{
	log(num);
	return *this;
}

Console& Console::operator<<(long long num)
{
	log(num);
	return *this;
}

Console& Console::operator<<(double num)
{
	log(num);
	return *this;
}

void Console::log(const char *ch)
{
	paint();
	if (_priority != ConsolePriority::Error) {
		cout << ch;
	} else {
		clog << ch;
	}

	end_paint();
}

void Console::log(const string& str)
{
	paint();

	if (_priority != ConsolePriority::Error) {
		cout << str;
	} else {
		clog << str;
	}

	end_paint();
}

void Console::log(int num)
{
	paint();

	if (_priority != ConsolePriority::Error) {
		cout << num;
	} else {
		clog << num;
	}

	end_paint();
}

void Console::log(long num)
{
	paint();

	if (_priority != ConsolePriority::Error) {
		cout << num;
	} else {
		clog << num;
	}

	end_paint();
}

void Console::log(long long num)
{
	paint();

	if (_priority != ConsolePriority::Error) {
		cout << num;
	} else {
		clog << num;
	}

	end_paint();
}

void Console::log(double num)
{
	paint();

	if (_priority != ConsolePriority::Error) {
		cout << num;
	} else {
		clog << num;
	}

	end_paint();
}

string Console::ask(const string& msg)
{
	string out;
	cout << msg + " ";
	getline(cin, out);
	cout << end();
	return out;
}

string Console::end()
{
	return "\n";
}

/*
Starts colouring new messages with given priority argument
*/
void Console::paint()
{
#if defined(_WIN32)
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (_priority) {
		case ConsolePriority::Verbose:
			SetConsoleTextAttribute(console, 8);
			break;
		case ConsolePriority::Warning:
			SetConsoleTextAttribute(console, 6);
			break;
		case ConsolePriority::Error:
			SetConsoleTextAttribute(console, 4);
			break;
		default:
			SetConsoleTextAttribute(console, 7);
			break;
	}
#elif defined(__unix__) or defined(__MACH__)
	using std::cout;

	switch (_priority) {
		case ConsolePriority::Verbose:
			cout << "\033[90m";
			break;
		case ConsolePriority::Warning:
			cout << "\033[33m";
			break;
		case ConsolePriority::Error:
			cout << "\033[31m";
			break;
		default:
			cout << "\033[0m";
			break;
	}
#endif
}

/*
Ends painting up and new messages will be coloured back to default
*/
void Console::end_paint()
{
#if defined(_WIN32)
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(console, &buffer_info);
	SetConsoleTextAttribute(console, buffer_info.wAttributes);
#elif defined(__unix__) or defined(__MACH__)
	using std::cout;
	cout << "\033[0m";
#endif
}
