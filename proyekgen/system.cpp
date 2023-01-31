#include "system.h"

/*
Returns if current user that executed this program is
an administrator or has root privileges.

This function uses low-level OS-underlying functions, if
the current OS doesn't have implementation, it returns false.
*/
bool SystemRuntime::is_admin_or_root()
{
#if defined(_WIN32)
	BOOL is_admin;
	PSID admin_groups = NULL;
	SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;

	is_admin = AllocateAndInitializeSid(&authority, 2,
		SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0, &admin_groups);

	if (is_admin) {
		if (!CheckTokenMembership(NULL, admin_groups, &is_admin)) {
			is_admin = FALSE;
		}

		FreeSid(admin_groups);
	}

	return is_admin;
#elif defined(__unix__) or defined(__MACH__)
	return geteuid() == 0;
#endif
	// By default, this function returns false if no implementation was coded for the current OS
	return false;
}

/*
Exits program with code.

Unused, this was used to terminate when log4cxx was used for logging.
*/
void SystemRuntime::fatal(int code)
{
	exit(code);
}

/*
Get the global configuration path.

On Windows, this returns the global_data_path function
meanwhile "/etc/proyekgen" on UNIX-based systems.
*/
string SystemBasePaths::global_config_path()
{
#if defined(_WIN32)
	// Just use the global_data_path function to store config
	return global_data_path();
#elif defined(__unix__) or defined(__MACH__)
	return "/etc/proyekgen";
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

/*
Get the global data path.

On Windows, this returns "%PROGRAMDATA%\proyekgen"
meanwhile "/var/lib/proyekgen" on UNIX-based systems.
*/
string SystemBasePaths::global_data_path()
{
#if defined(_WIN32)
	wchar_t *path = new wchar_t[260];
	HRESULT code = E_FAIL;
	code = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &path);

	if (code == E_FAIL) {
		LOG(FATAL) << "Cannot find ProgramData. (code " << code << ")";
	}

	wstringstream stream;
	wstring w_result;
	string result;
	stream << path << separator << "proyekgen";
	CoTaskMemFree(static_cast<void*>(path));
	w_result = stream.str();

	transform(w_result.begin(), w_result.end(), back_inserter(result), [](wchar_t c) {
		return (char)c;
	});

	return result;
#elif defined(__unix__) or defined(__MACH__)
	return "/var/lib/proyekgen";
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

/*
Get the global template path.

This returns the global_data_path function
with the preferred path separator and "templates"
*/
string SystemBasePaths::global_templates_path()
{
	return global_data_path() + separator + "templates";
}

/*
Get the local configuration path.

Unlike global_config_path, this function returns the local_data_path function
with the preferred path separator and "config"
*/
string SystemBasePaths::local_config_path()
{
#if defined(_WIN32)
	// Just use LocalAppData to store config files
	return local_data_path() + "\\config";
#elif defined(__unix__) or defined(__MACH__)
	const char *path = getenv("XDG_CONFIG_HOME");
	const char *fallback_path = getenv("HOME");
	stringstream stream;

	if (path != nullptr) {
		stream << path;
	} else if (fallback_path != nullptr) {
		stream << fallback_path;
	} else {
		LOG(FATAL) << "Cannot reserve local configuration files.";
	}

	stream << "/.proyekgen/config";
	return stream.str();
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

/*
Get the local data path.

On Windows, this returns "%APPDATA%\proyekgen"
meanwhile respective XDG directory specification (or
"$HOME/.proyekgen" in fallback) on UNIX-based systems.
*/
string SystemBasePaths::local_data_path()
{
#if defined(_WIN32)
	wchar_t *path = 0;
	HRESULT code = E_FAIL;
	code = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);

	if (code == E_FAIL) {
		LOG(FATAL) << "Cannot find AppData. (code " << code << ")";
	}

	wstringstream stream;
	wstring w_result;
	string result;
	stream << path << separator << "proyekgen";
	CoTaskMemFree(static_cast<void*>(path));
	w_result = stream.str();

	transform(w_result.begin(), w_result.end(), back_inserter(result), [](wchar_t c) {
		return (char)c;
		});

	return result;
#elif defined(__unix__) or defined(__MACH__)
	const char *path = getenv("XDG_DATA_HOME");
	const char *fallback_path = getenv("HOME");
	stringstream stream;

	if (path != nullptr) {
		stream << path;
	} else if (fallback_path != nullptr) {
		stream << fallback_path;
	} else {
		LOG(FATAL) << "Cannot reserve local data files.";
	}

	stream << "/.proyekgen";
	return stream.str();
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

/*
Get the local template path

This returns the local_data_path function
with the preferred path separator and "templates"
*/
string SystemBasePaths::local_templates_path()
{
	return local_data_path() + separator + "templates";
}

/*
Get the executable path

This function uses low-level OS-underlying functions, if
the current OS doesn't have implementation, it returns an empty string.
*/
string SystemPaths::executable_path()
{
#if defined(_WIN32)
	wchar_t path[MAX_PATH] = {0};
	wstring w_result;
	string result;
	GetModuleFileNameW(NULL, path, MAX_PATH);
	w_result = path;

	transform(w_result.begin(), w_result.end(), back_inserter(result), [](wchar_t c) {
		return (char)c;
	});

	return result;
#elif defined(__unix__) or defined(__MACH__)
	char p[PATH_MAX] = {};
	ssize_t c = readlink("/proc/self/exe", p, PATH_MAX);
	return string(p, (c > 0) ? c : 0);
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

/*
Get the current working path
*/
string SystemPaths::current_path()
{
	return filesystem::current_path().string();
}

/*
Get the list of configuration paths

If the current user is not an admin / has no root privileges,
the first path (which is SystemBasePaths::global_config_path) is an empty string.
*/
vector<string> SystemPaths::config_paths()
{
	return {SystemBasePaths::global_config_path(), SystemBasePaths::local_config_path()};
}

/*
Get the list of data paths

If the current user is not an admin / has no root privileges,
the first path (which is SystemBasePaths::global_data_path) is an empty string.
*/
vector<string> SystemPaths::data_paths()
{
	return {SystemBasePaths::global_data_path(), SystemBasePaths::local_data_path()};
}

/*
Get the list of template paths

If the current user is not an admin / has no root privileges,
the first path (which is SystemBasePaths::global_templates_path) is an empty string.
*/
vector<string> SystemPaths::template_paths()
{
	return {SystemBasePaths::global_templates_path(),
		SystemBasePaths::local_templates_path(),
		current_path() + separator + ".proyekgen"};
}
