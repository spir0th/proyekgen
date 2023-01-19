#include "console.h"

int main(int argc, char *argv[])
{
	Console() << "Begin console test:" << newline;
	Console(MessagePriority::Normal) << "	This is a normal message" << newline;
	Console(MessagePriority::Debug) << "	This is a debug message, the \"--debug\" command-line option will reveal the messages that have this priority." << newline;
	Console(MessagePriority::Warning) << "	This is a warning message, the \"--no-warn\" command-line option will suppress these warnings." << newline;
	Console(MessagePriority::Error) << "	This is an error message, the \"--skip-errors\" command-line option will make the program continue running even there are errors." << newline;
	Console() << "End console test..." << newline;
	return 0;
}