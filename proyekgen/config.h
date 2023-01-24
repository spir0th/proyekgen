#pragma once
#include "console.h"
#include "error.h"
#include "global.h"
#include "system.h"

/*
A helper class to locate config files.
*/
class ConfigPaths
{
public:
	static string generator();
	static string global();
	static string templates();
	static string version();
};

/*
The application config class, it is used in most classes to utilize settings.
*/
class Config
{
public:
	static json generator();
	static json global();
	static json templates();
	static json version();
};