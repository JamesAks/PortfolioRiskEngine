# include "../include/Adapter.hpp"



AlphaVantageAdapter::AlphaVantageAdapter(std::string key) : API_key{ key }{}


std::string AlphaVantageAdapter::request(std::string url) {

	CURL* curl;
	CURLcode result;
	std::string response;

	curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, memoryWriteCallback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

	result = curl_easy_perform(curl);
	curl_easy_cleanup(curl);

	return response;
}


std::pair<std::vector<std::string>, std::vector<double>>  AlphaVantageAdapter::historicalDaily(std::string symbol) {

	std::string hey = base_url + symbol;

	std::string url = base_url + std::string("function=TIME_SERIES_DAILY&symbol=") + symbol + std::string("&apikey=") + API_key;
	printf("%s", url.c_str());

	std::string response = request(url);
	auto response_json = parse(response);

	auto& data = response_json["Time Series (Daily)"];
	std::pair<std::vector<std::string>, std::vector<double>>  historical_data;

	for (auto& [date, daily_data] : data.items()) {

		double price = std::stod(std::string(daily_data["4. close"]));
		historical_data.first.push_back(date);
		historical_data.second.push_back(price);
	}

 	return historical_data;
}


std::pair<std::vector<std::string>, std::vector<double>> AlphaVantageAdapter::historicalMonthly(std::string) {

	std::pair<std::vector<std::string>, std::vector<double>> ph;

	return ph;
}

std::string AlphaVantageAdapter::formURL(std::string ) {

	return "";
}


nlohmann::json AlphaVantageAdapter::parse(std::string response){

	return nlohmann::json::parse(response);
}


size_t memoryWriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {

	((std::string*)userp)->append((char*)contents, size * nmemb);
	return size * nmemb;
}