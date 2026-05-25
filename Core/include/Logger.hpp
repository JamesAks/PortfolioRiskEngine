#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <iostream>
#include <string.h>
#include <chrono>
#include <sstream>

enum class LogLevel { ERR, WARNING, INFO, DEBUG};


class Logger{
	
	// Utility for logging errors, warnings etc to the output terminal.
	private:

		static std::string timestamp();

	public:

		static void logError(std::string);
		static void logWarning(std::string) ;
		static void logInfo(std::string) ;
		static void logDebug(std::string);
};

#endif // !LOGGER_HPP