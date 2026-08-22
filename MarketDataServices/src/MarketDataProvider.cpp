#include "Logger.hpp"
#include "MarketDataProvider.hpp"

// ----- Private Members -----

// ----- Public Members -----

std::chrono::year_month_day convertStringDate(const std::string& date) {

	auto first_seperator = date.find("-");
	auto second_sperator = date.find("-", first_seperator + 1);

	//Logger::logDebug("Second Location: " + std::to_string(second_sperator));

	std::chrono::year date_year(std::stoi(date.substr(0, first_seperator)));
	std::chrono::month date_month(std::stoi(date.substr(first_seperator + 1, second_sperator)));
	std::chrono::day date_day(std::stoi(date.substr(second_sperator + 1, date.size())));

	return { date_year, date_month, date_day };
}

