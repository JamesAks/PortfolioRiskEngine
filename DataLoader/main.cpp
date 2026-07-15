#include <fstream>
#include <iostream>
#include <filesystem>

#include "AlphaVantageProvider.hpp"



static std::string DIR_PATH = "../../../../../DataLoader/TestMarketData/";


static void writeToCSV(std::string asset_ID, std::string file_name, const TimeSeries& time_series) {

	std::string asset_dir_path = DIR_PATH + asset_ID;
	if (!std::filesystem::is_directory(asset_dir_path)) {

		std::filesystem::create_directory(asset_dir_path);
	}

	// ofstream creates file in the directory if ther is not already one.
	std::ofstream market_data_file(asset_dir_path + "/" + file_name);
	if (!market_data_file.is_open()) {
		throw "Market data file not found.";
	}

	for (int i = 0; i < time_series.dates().size(); i++) {

		market_data_file << time_series.dates()[i] << "," << time_series.prices()[i] << std::endl;
		Logger::logDebug("Added price #" + std::to_string(i) + " to " + asset_ID + "'s " + file_name);
	}

	market_data_file.close();
}


static void loadMarketData(std::string asset_ID, size_t quantity) {

	AlphaVantageProvider avp(std::getenv("ALPHA_VANTAGE_API_KEY"));
	std::string file_name;

	auto daily_result = avp.periodicData(asset_ID, TimeFrame::DAILY,quantity);
	if (daily_result.request_error != RequestError::NONE) {

		Logger::logError("Could not get daily data for \"" + asset_ID + "\".");
		return;
	}

	auto weekly_result = avp.periodicData(asset_ID, TimeFrame::WEEKLY, quantity);
	if (weekly_result.request_error != RequestError::NONE) {

		Logger::logError("Could not get weekly data for \"" + asset_ID + "\".");
		return;
	}

	auto monthly_result = avp.periodicData(asset_ID, TimeFrame::MONTHLY, quantity);
	if (monthly_result.request_error != RequestError::NONE) {

		Logger::logError("Could not get monthly data for \"" + asset_ID + "\".");
		return;
	}

	auto latest_price_result = avp.latestPrice(asset_ID);
	if (latest_price_result.request_error != RequestError::NONE) {

		Logger::logError("Could not get latest_price for \"" + asset_ID + "\".");
		return;
	}

	writeToCSV(asset_ID, "Daily_Market_Data.csv", daily_result.time_series.value());
	writeToCSV(asset_ID, "Weekly_Market_Data.csv", weekly_result.time_series.value());
	writeToCSV(asset_ID, "Monthly_Market_Data.csv", monthly_result.time_series .value());

	std::ofstream market_data_file(DIR_PATH + asset_ID + "/Latest_Price.csv");
	if (!market_data_file.is_open()) {
		throw "Market data file not found.";
	}

	market_data_file << latest_price_result.price.timestamp() << "," << latest_price_result.price.price() << std::endl;

	market_data_file.close();
}


int main(){

	int finished = 0;
	while (!finished) {

		std::string asset_ID;
		size_t quantity = 0;

		std::cout << "Market data ticker?" << std::endl;
		std::cin >> asset_ID;

		std::cout << "Quantity?" << std::endl;
		std::cin >> quantity;

		loadMarketData(asset_ID, quantity);

		std::cout << "Input 0 to load more data or 1 to end." << std::endl;
		std::cin >> finished;
	}
}