#include "CSVDataProvider.hpp"

#include <iostream>

static std::string DIR_PATH = "../../../../../../DataLoader/TestMarketData";

static void testCSVDataProvider() {

	CSVDataProvider csv_provider(DIR_PATH);
	auto data = csv_provider.periodicData("AAPL", TimeFrame::DAILY, 100);
	if (data.requestError != RequestError::NONE) {

		Logger::logDebug("Error getting historic data.");
		return;
	}

	auto& hd = data.historicData.value();

	auto& dates = hd.dates();
	auto& prices = hd.prices();

	for (int i = 0; i < 100; i++) {

		std::cout << "Date: " << dates[i] << " ---> Price: " << prices[i] << std::endl; 
	}

	auto lp = csv_provider.latestPrice("AAPL");
	if (lp.requestError != RequestError::NONE) {

		Logger::logDebug("Error getting latest price.");
		return;
	}

	std::cout << "Latest Price: " << lp.price.price();
}


int main() {

	testCSVDataProvider();

	

}