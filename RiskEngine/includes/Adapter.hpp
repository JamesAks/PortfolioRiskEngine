#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include "Logger.hpp"

enum class TimeFrame { DAILY, WEEKLY, MONTHLY };
enum class RequestError { NONE, INVALIDSYMBOL, RATELIMIT, NETWORKERROR };

struct TimeSeries {

	// Struct that holds the actual historical data of an asset
	std::vector<std::string> dates;
	std::vector<double> prices;
};


struct RequestResult {

	std::optional<TimeSeries> historicData;
	RequestError requestError;
	double price;
};
#include <iostream>

class Adapter {

	public:

		virtual RequestResult periodicData(std::string, TimeFrame) const = 0;
		virtual RequestResult latestPrice(std::string) const = 0;
};


class AlphaVantageAdapter: public Adapter {

	private:

		std::string API_key;
		std::string base_url = "https://www.alphavantage.co/query?";
	 
		std::string request(std::string) const;
		RequestError validateResponse(std::string, const nlohmann::json&) const;

	public:

		AlphaVantageAdapter(std::string);
		
		RequestResult periodicData(std::string, TimeFrame) const;
		RequestResult latestPrice(std::string) const;


		nlohmann::json parse(std::string) const;
};

static size_t memoryWriteCallback(void*, size_t, size_t, void*);


