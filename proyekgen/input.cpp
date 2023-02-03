#include "input.h"

/*
 * Creates a new input and returns the value when user is done.
*/
string input(const string &msg)
{
	using std::cin;
	string output;
	fmt::print(msg);
	getline(cin, output);
	return output;
}