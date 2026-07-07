#include "AlphaVantageProvider.hpp"
#include "MarketData.hpp"
#include "Logger.hpp"
#include "TimeSeries.hpp"


#include <chrono>
#include <curl/curl.h>
#include <thread>

// ----- Private Members -----

std::string AlphaVantageProvider::request(std::string url) const {

	CURL* curl;
	CURLcode result;
	std::string response;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memoryWriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	//Logger::logDebug("Waiting one second for rate limit");
	std::this_thread::sleep_for(std::chrono::seconds(1));
	//Logger::logDebug("Waiting finished");      

	return response;
}


RequestError AlphaVantageProvider::validateResponse(std::string symbol, const nlohmann::json& response_json) const {

	if (response_json.contains("Error Message")) {

		Logger::logError("Could not fetch data. \"" + symbol + "\" is an invalid symbol. Alpha Vanatge error message: \n" + response_json["Error Message"].get<std::string>());
		return RequestError::INVALIDSYMBOL;
	}

	if (response_json.contains("Note")) {

		Logger::logError("Could not fetch data. Rate limit reached for this adapter. Alpha Vantage Error Message: \n" + response_json["Note"].get<std::string>());
		return RequestError::RATELIMIT;
	}

	if (response_json.contains("Information")) {

		Logger::logError("Could not fetch data. Rate limit reached for this adapter. Alpha Vantage Error Message: \n" + response_json["Information"].get<std::string>());
		return RequestError::RATELIMIT;
	}

	return RequestError::NONE;
}

// ----- Public Members -----

AlphaVantageProvider::AlphaVantageProvider(std::string key) : API_key{ key } {}


RequestResult AlphaVantageProvider::periodicData (std::string symbol, TimeFrame tf, size_t quantity) const  {

	std::string function;
	TimeSeries  periodic_data(quantity);
	std::string title;

	switch (tf) {

		case TimeFrame::DAILY:

			function = "TIME_SERIES_DAILY";
			title = "Time Series (Daily)";
			Logger::logInfo("Fetching daily data for \"" + symbol + "\".");
			break;

		case TimeFrame::WEEKLY:

			function = "TIME_SERIES_WEEKLY";
			title = "Weekly Time Series";
			Logger::logInfo("Fetching weekly data for \"" + symbol + "\".");
			break;

		case TimeFrame::MONTHLY:

			function = "TIME_SERIES_MONTHLY";
			title = "Monthly Time Series";
			Logger::logInfo("Fetching monthly data for \"" + symbol + "\".");
			break;

		default:

			throw std::runtime_error("Missing TimeFrame. ");
	}

	std::string url = base_url + std::string("function=") + function + std::string("&symbol=") + symbol + std::string("&apikey=") + API_key;
	std::string response = request(url);
	auto response_json = parse(response);

	auto vr = validateResponse(symbol, response_json);
	if (vr != RequestError::NONE) { return { std::nullopt, vr }; }
	
	if (!response_json.contains(title.c_str())) { throw std::runtime_error("Missing market data for \"" + symbol + "\"."); }

	auto& data = response_json[title.c_str()];

	Logger::logDebug("Data size: " + std::to_string(data.size()));

	if (data.size() < quantity) {

		quantity = data.size();
		Logger::logWarning("The quantity requested was more than what was provided. Returning" + std::to_string(quantity) + " entrys.");
	}
	
	int count = 0;
	for (auto& [date, daily_data] : data.items()) {

		if (count >= quantity) { break; }

		if (!daily_data.contains("4. close")) { throw std::runtime_error("Missing prices for \"" + symbol + "\"."); }
		double price = std::stod(daily_data["4. close"].get<std::string>());
		periodic_data.addData(convertStringDate(date), price);
		count++;
	}

	return { periodic_data, RequestError::NONE};
}


RequestResult AlphaVantageProvider::latestPrice(std::string symbol) const {

	Logger::logInfo("Fethcing Latest data for \"" + symbol + "\".");

	std::string url = base_url + std::string("function=GLOBAL_QUOTE&symbol=") + symbol + std::string("&apikey=") + API_key;
	std::string response = request(url);
	auto response_json = parse(response);

	auto vr = validateResponse(symbol, response_json);
	if (vr != RequestError::NONE) { return { std::nullopt, vr }; }

	if (!response_json.contains("Global Quote")) { throw std::runtime_error("Missing global quote."); }

	auto& data = response_json["Global Quote"];

	if (!data.contains("05. price")) { throw std::runtime_error("Missing price."); }

	LatestPrice price{

		std::stod(data["05. price"].get<std::string>()),
		convertStringDate(data["07. latest trading day"])
	};

	RequestResult result{

		std::nullopt,
		RequestError::NONE,
		price
	};

	return result;
}


nlohmann::json AlphaVantageProvider::parse(std::string response) const {

	return nlohmann::json::parse(response);
}


static size_t memoryWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {

	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}
