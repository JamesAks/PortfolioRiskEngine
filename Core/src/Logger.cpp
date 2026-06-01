#include "Logger.hpp"

#include <iostream>
#include <chrono>
#include <sstream>

void Logger::logError(std::string message) {

	std::cout << timestamp() << "[ERROR] " << message << "\n";
}


void Logger::logWarning(std::string message) {

	std::cout << timestamp() << "[WARNING] " << message << "\n";
}


void Logger::logInfo(std::string message) {

	std::cout << timestamp() << "[INFO] " << message << "\n";
}

void Logger::logDebug(std::string message) {

	std::cout << timestamp() << "[DEBUG] " << message << "\n";
}


std::string Logger::timestamp(){

	auto time = std::chrono::system_clock::now();
	auto now = std::chrono::system_clock::to_time_t(time);
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch());
	std::tm now_time = *std::localtime(&now);
	std::ostringstream oss;
	oss << std::put_time(&now_time, "%H:%M:%S") << /*":" << std::setw(3) << ms.count() <<*/ " - ";
	
	return oss.str();
	 
 }