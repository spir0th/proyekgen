#include "logger.h"

auto get_logger(const string& name) -> log4cxx::LoggerPtr
{
	static struct log4cxx_initializer {
		log4cxx_initializer() {
			log4cxx::PropertyConfigurator::configure("proyekgen.properties");
		}
		~log4cxx_initializer() {
			log4cxx::LogManager::shutdown();
		}
	} initAndShutdown;

	return name.empty() ? log4cxx::LogManager::getRootLogger()
		: log4cxx::LogManager::getLogger(name);
}