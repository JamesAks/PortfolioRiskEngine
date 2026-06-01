#include "RiskEngine.hpp"
#include "GenericDataStore.hpp"
#include "CSVDataProvider.hpp"
#include "Portfolio.hpp"
#include "Logger.hpp"

#include <iostream>

static std::string DIR_PATH = "../../../../../../DataLoader/TestMarketData";


int main() {
	 
	GenericDataStore market_data_store;
	CSVDataProvider market_data_provider(DIR_PATH);
	RiskEngine test_risk_engine;
	Portfolio test_portfolio("TestPortfolio1");

	auto lp = market_data_provider.latestPrice("AAPL");

	if (lp.requestError != RequestError::NONE) {

		Logger::logError("Could not get latest price for \"APPL\".");
		return 1;
	}

	auto daily = market_data_provider.periodicData("AAPL", TimeFrame::DAILY, 100);
	if (daily.requestError != RequestError::NONE) {

		Logger::logError("Could not get daily data price for \"APPL\".");
		return 1;
	}

	auto weekly = market_data_provider.periodicData("AAPL", TimeFrame::WEEKLY, 100);
	if (weekly.requestError != RequestError::NONE) {

		Logger::logError("Could not get weekly data price for \"APPL\".");
		return 1;
	}
	auto monthly = market_data_provider.periodicData("AAPL", TimeFrame::MONTHLY, 100);
	if (monthly.requestError != RequestError::NONE) {

		Logger::logError("Could not get monthly data price for \"APPL\".");
		return 1;
	}

	
	market_data_store.addHistoricalData("AAPL",daily.historicData.value(), weekly.historicData.value(), monthly.historicData.value());
	market_data_store.addLatestPrice("AAPL", lp.price);

	for (auto value : market_data_store.periodicData("AAPL", TimeFrame::DAILY).prices()) {

		std::cout << value << std::endl;

	}

	market_data_store.updateHistoricData("AAPL", daily.historicData.value(), weekly.historicData.value(), monthly.historicData.value());


	//auto test1 = std::make_shared<Asset>("AAPL", lp, hd);
	//Position testPosition("TestPosition1", 10, test1, 100, PositionType::LONG);

	//test_portfolio.addPosition(testPosition);
	//test_risk_engine.analysePortfolio(test_portfolio, TimeFrame::DAILY);


}  