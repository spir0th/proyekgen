#include "console.h"

Console::Console(MessagePriority priority)
	: _priority(priority) 
{
	init();
}

Console::Console()
	: _priority(MessagePriority::Normal)
{
	init();
}

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

Console& Console::operator<<(double num)
{
	log(num);
	return *this;
}

Console& Console::operator>>(pair<const string&, string> args)
{
	input(args.first, args.second);
	return *this;
}

Console& Console::operator>>(pair<const string&, int> args)
{
	input(args.first, args.second);
	return *this;
}

void Console::log(const char *ch)
{
	paint();
	std::cout << ch;
	end_paint();
}

void Console::log(const string& str)
{
	paint();
	std::cout << str;
	end_paint();
}

void Console::log(int num)
{
	paint();
	std::cout << num;
	end_paint();
}

void Console::log(long num)
{
	paint();
	std::cout << num;
	end_paint();
}

void Console::log(double num)
{
	paint();
	std::cout << num;
	end_paint();
}

void Console::input(const string& msg, string out)
{
	std::cout << msg;
	std::getline(std::cin, out);
}

void Console::input(const string& msg, int out)
{
	std::cout << msg;
	std::cin >> out;
}

string Console::end()
{
	return "\n";
}

void Console::init()
{
	switch (_priority) {
		case MessagePriority::Debug:
			tag = "[DEBUG]";
			break;
		case MessagePriority::Warning:
			tag = "[WARNING]";
			break;
		case MessagePriority::Error:
			tag = "[ERROR]";
			break;
		default:
			tag = "[INFO]";
			break;
	}
}

/*
Starts colouring new messages with given priority argument
*/
void Console::paint()
{
#if defined(_WIN32)
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	switch (_priority) {
		case MessagePriority::Debug:
			SetConsoleTextAttribute(console, 8);
			break;
		case MessagePriority::Warning:
			SetConsoleTextAttribute(console, 6);
			break;
		case MessagePriority::Error:
			SetConsoleTextAttribute(console, 4);
			break;
		default:
			SetConsoleTextAttribute(console, 7);
			break;
	}
#elif defined(__unix__) or defined(__MACH__)
	using std::cout;

	switch (_priority) {
		case MessagePriority::Debug:
			cout << "\033[90m";
			break;
		case MessagePriority::Warning:
			cout << "\033[33m";
			break;
		case MessagePriority::Error:
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
