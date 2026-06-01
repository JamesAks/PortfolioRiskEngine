#include "CSVDataProvider.hpp"

static std::string DIR_PATH = "../../../../../../DataLoader/TestMarketData";

static void testCSVDataProvider() {

	CSVDataProvider csv_provider(DIR_PATH);
	auto data = csv_provider.periodicData("AAPL", TimeFrame::DAILY, 100);
	if (data.requestError != RequestError::NONE) {

		Logger::logDebug("Error getting historic data.");
		return;
	}

	auto& hd = data.historicData.value();

	for (int i = 0; i < 100; i++) {

		std::cout << "Date: " << hd.dates[i] << " ---> Price: " << hd.prices[i] << std::endl; 
	}

	auto latestPrice = csv_provider.latestPrice("AAPL");
	if (latestPrice.requestError != RequestError::NONE) {

		Logger::logDebug("Error getting latest price.");
		return;
	}

	std::cout << "Latest Price: " << latestPrice.price;


}

int main() {

	testCSVDataProvider();

	

}