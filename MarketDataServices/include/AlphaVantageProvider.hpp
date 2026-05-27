
#ifndef MARKET_DATA_PROVIDER_HPP
#define MARKET_DATA_PROVIDER_HPP

#include <curl/curl.h>
#include <nlohmann/json.hpp>
#include <chrono>
#include <thread>
#include "Logger.hpp"
#include "Core.hpp"
#include "MarketDataProvider.hpp"


class AlphaVantageProvider: MarketDataProvider {

	private:

		std::string API_key;
		std::string base_url = "https://www.alphavantage.co/query?";
	 
		std::string request(std::string) const;
		RequestError validateResponse(std::string, const nlohmann::json&) const;
		nlohmann::json parse(std::string) const;

	public:

		AlphaVantageProvider(std::string);
		
		RequestResult periodicData(std::string, TimeFrame, size_t) const;
		RequestResult latestPrice(std::string) const;


		
};

static size_t memoryWriteCallback(void*, size_t, size_t, void*);

#endif // !MARKET_DATA_PROVIDER


