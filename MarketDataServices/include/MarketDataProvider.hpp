#ifndef MARKET_DATA_PROVIDER_INTERFACE_HPP
#define MARKET_DATA_PROVIDER_INTERFACE_HPP

#include "TimeSeries.hpp"
#include "HistoricData.hpp"
#include "Logger.hpp"

#include <chrono>
#include <optional>
#include <string>

enum class RequestError { NONE, INVALIDSYMBOL, RATELIMIT, NETWORKERROR };

struct RequestResult {

	std::optional<TimeSeries> historicData;
	RequestError requestError;
	double price;
};


class MarketDataProvider {

	public:

		virtual RequestResult periodicData(std::string, TimeFrame, size_t) const = 0;
		virtual RequestResult latestPrice(std::string) const = 0;
};

static std::chrono::year_month_day convertStringDate(const std::string& date) {

	auto first_seperator = date.find("-");
	auto second_sperator = date.find("-", first_seperator + 1);

	//Logger::logDebug("Second Location: " + std::to_string(second_sperator));

	std::chrono::year date_year(std::stoi(date.substr(0, first_seperator)));
	std::chrono::month date_month(std::stoi(date.substr(first_seperator + 1, second_sperator)));
	std::chrono::day date_day(std::stoi(date.substr(second_sperator + 1, date.size())));

	return { date_year, date_month, date_day };
}

#endif // !MARKET_DATA_PROVIDER_INTERFACE_HPP