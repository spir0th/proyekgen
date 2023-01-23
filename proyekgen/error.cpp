#include "error.h"

LibArchiveInternalError::LibArchiveInternalError(const string &what)
	: runtime_error(what)
{}

ProjectFileNotFoundError::ProjectFileNotFoundError(const string &what)
	: filesystem_error(what, std::error_code())
{}

TemplateNotFoundError::TemplateNotFoundError(const string &what)
	: filesystem_error(what, std::error_code())
{}

UnixNoPathPreservedError::UnixNoPathPreservedError(const string &what)
	: filesystem_error(what, std::error_code())
{}

Win32InternalError::Win32InternalError(const string &what)
	: runtime_error(what)
{}
