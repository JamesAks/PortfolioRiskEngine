# include "../include/Adapter.hpp"



AlphaVantageAdapter::AlphaVantageAdapter(std::string key) : API_key{ key }{}


std::string AlphaVantageAdapter::request(std::string url) const {

	CURL* curl;
	CURLcode result;
	std::string response;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memoryWriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	std::this_thread::sleep_for(std::chrono::seconds(1));

	return response;
}



RequestResult AlphaVantageAdapter::historicalData (std::string symbol, TimeFrame tf) const  {

	std::string function;
	TimeSeries  historical_data;
	std::string title;

	switch (tf) {

		case TimeFrame::DAILY:

			function = "TIME_SERIES_DAILY";
			title = "Time Series (Daily)";
			break;

		case TimeFrame::WEEKLY:

			function = "TIME_SERIES_WEEKLY";
			title = "Weekly Time Series";
			break;

		case TimeFrame::MONTHLY:

			function = "TIME_SERIES_MONTHLY";
			title = "Monthly Time Series";
			break;
	}


	std::string url = base_url + std::string("function=") + function + std::string("&symbol=") + symbol + std::string("&apikey=") + API_key;
	std::string response = request(url);
	auto response_json = parse(response);

	if (response_json.contains("Error Message")) {

		RequestResult result{

			std::nullopt,
			RequestError::INVALIDSYMBOL,
			response_json["Error Message"]
		};

		return result;
	}

	if (response_json.contains("Note")) {

		RequestResult result{

			std::nullopt,
			RequestError::RATELIMIT,
			response_json["Note"]
		};

		return result;
	}


	auto& data = response_json[title.c_str()];
	for (auto& [date, daily_data] : data.items()) {

		double price = std::stod(std::string(daily_data["4. close"]));
		historical_data.dates.push_back(date);
		historical_data.prices.push_back(price);
	}

	RequestResult result{

		historical_data,  
		RequestError::NONE,
		"Successfully retrieved data",
		0
	};

	return result;

}


RequestResult AlphaVantageAdapter::latestPrice(std::string symbol) const {

	std::string url = base_url + std::string("function=GLOBAL_QUOTE&symbol=") + symbol + std::string("&apikey=") + API_key;
	
	
	std::string response = request(url);
	std::cout << response;
	auto response_json = parse(response);

	if (response_json.contains("Error Message")) {

		RequestResult result{

			std::nullopt,
			RequestError::INVALIDSYMBOL,
			response_json["Error Message"],
			0
		};

		return result;
	}

	if (response_json.contains("Note")) {

		RequestResult result{

			std::nullopt,
			RequestError::RATELIMIT,
			response_json["Note"],
			0
		};

		return result;
	}

	auto& data = response_json["Global Quote"];
	double price = std::stod(data["05. price"].get<std::string>());
	
	RequestResult result{

		std::nullopt,
		RequestError::NONE,
		"Successfully retrieved data.",
		price
	};

	return result;
}


nlohmann::json AlphaVantageAdapter::parse(std::string response) const {

	return nlohmann::json::parse(response);
}


size_t memoryWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {

	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}