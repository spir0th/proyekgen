#include "system.h"

vector<string> SystemRuntime::args()
{
	vector<string> args;

	if (_argc > 1) {
		args.assign(_argv + 1, _argv + _argc);
	}

	return args;
}

void SystemRuntime::catch_termination()
{
	set_terminate([]() {
		exception_ptr ptr = current_exception();
		
		try {
			rethrow_exception(ptr);
		} catch (exception ex) {
			print_error << ex.what() << newline;
			print << ""; // Reset current console text color
		}

		std::abort(); // Accessing this function from std namespace
	});
}

void SystemRuntime::init(int argc, char *argv[])
{
	_argc = argc;
	_argv = argv;
}

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
	uid_t public_uid = getuid();
	uid_t private_uid = geteuid();
	return public_uid != private_uid;
#endif
	// By default, this function returns false if no implementation was coded for the current OS
	return false;
}

string SystemPaths::executable_path()
{
	return _argv[0]; // We use command-line arguments to get the path.
}

string SystemPaths::config_path()
{
	if (!SystemRuntime::is_admin_or_root()) {
		return local_config_path();
	} else {
		return system_config_path();
	}
}

string SystemPaths::data_path()
{
	if (!SystemRuntime::is_admin_or_root()) {
		return local_data_path();
	} else {
		return system_data_path();
	}
}

vector<string> SystemPaths::template_paths()
{
	return {system_templates_path(), local_templates_path(),
		filesystem::current_path().string() + path_separator + ".proyekgen"};
}

string SystemPaths::local_config_path()
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
		throw UnixNoPathPreservedError("Unable to locate local config files.");
	}

	stream << "/.proyekgen/config";
	return stream.str();
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

string SystemPaths::system_config_path()
{
#if defined(_WIN32)
	// Just use ProgramData to store config files
	return system_data_path() + "\\config";
#elif defined(__unix__) or defined(__MACH__)
	// Every *nix system has a /etc directory
	return "/etc/proyekgen";
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

string SystemPaths::local_data_path()
{
#if defined(_WIN32)
	wchar_t *path = 0;
	HRESULT code = E_FAIL;
	code = SHGetKnownFolderPath(FOLDERID_LocalAppData, 0, NULL, &path);

	if (code == E_FAIL) {
		// Failure of finding the LocalAppData location
		throw Win32InternalError("Failed to find the location of LocalAppData.");
	}

	wstringstream stream;
	wstring w_result;
	string result;
	stream << path << path_separator << "proyekgen";
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
		throw UnixNoPathPreservedError("Unable to locate local data files.");
	}

	stream << "/.proyekgen";
	return stream.str();
#endif
	// If OS has no specific implementation, return an empty string.
	return string();
}

string SystemPaths::system_data_path()
{
#if defined(_WIN32)
	wchar_t *path = new wchar_t[260];
	HRESULT code = E_FAIL;
	code = SHGetKnownFolderPath(FOLDERID_ProgramData, 0, NULL, &path);

	if (code == E_FAIL) {
		// Failure of finding the ProgramData location
		throw Win32InternalError("Failed to find the location of ProgramData.");
	}

	wstringstream stream;
	wstring w_result;
	string result;
	stream << path << path_separator << "proyekgen";
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

string SystemPaths::local_templates_path()
{
	return local_data_path() + path_separator + "templates";
}

string SystemPaths::system_templates_path()
{
	return system_data_path() + path_separator + "templates";
}
