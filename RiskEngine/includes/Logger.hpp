#include <iostream>
#include <string.h>
#include <chrono>
#include <sstream>

enum class LogLevel { ERR, WARNING, INFO, DEBUG};


class Logger{

	private:

		static std::string timestamp();

	public:

		static void logError(std::string);
		static void logWarning(std::string) ;
		static void logInfo(std::string) ;
		static void logDebug(std::string);
		
	

};

