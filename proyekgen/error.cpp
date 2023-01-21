#include "error.h"

RequiredConfigNotFoundError::RequiredConfigNotFoundError(const string& what)
	: filesystem_error(what, std::error_code())
{}

Win32InternalError::Win32InternalError(const string& what)
	: runtime_error(what)
{}