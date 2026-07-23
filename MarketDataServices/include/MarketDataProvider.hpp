#ifndef MARKET_DATA_PROVIDER_INTERFACE_HPP
#define MARKET_DATA_PROVIDER_INTERFACE_HPP

#include "TimeSeries.hpp"
#include "MarketData.hpp"
#include "Logger.hpp"

#include <chrono>
#include <optional>
#include <string>



enum class RequestError { NONE, INVALIDSYMBOL, RATELIMIT, NETWORKERROR };

struct RequestResult {

	std::optional<TimeSeries> time_series;
	RequestError request_error;
	LatestPrice price;
};


class MarketDataProvider {

	public:

		virtual RequestResult periodicData(std::string, TimeFrame, size_t) const = 0;
		virtual RequestResult latestPrice(std::string) const = 0;
		virtual ~MarketDataProvider() = default;
		
		
};


std::chrono::year_month_day convertStringDate(const std::string&);


#endif // !MARKET_DATA_PROVIDER_INTERFACE_HPP