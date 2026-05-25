#include "RiskEngine.hpp"
#include "GenericDataStore.hpp"
#include "AlphaVantageProvider.hpp"


void main() {
	 
	GenericDataStore market_data_store;
	AlphaVantagProvider market_data_provider(getenv("ALPHA_VANTAGE_API_KEY"));
	RiskEngine test_risk_engine;
	Portfolio test_portfolio("TestPortfolio1");

	auto hd = std::make_shared<HistoricData>();

	HistoricData hd;

	hd->daily = market_data_provider.periodicData("AAPL", TimeFrame::DAILY).historicData.value();
	hd->weekly = market_data_provider.periodicData("AAPL", TimeFrame::WEEKLY).historicData.value();
	hd->monthly = market_data_provider.periodicData("AAPL", TimeFrame::MONTHLY).historicData.value();

	auto lp = std::make_shared<double>(market_data_provider.latestPrice("AAPL").price);

	market_data_store.addHistoricalData("AAPL", *hd);
	market_data_store.addLatestPrice("AAPL", *lp);


	auto test1 = std::make_shared<Asset>("AAPL", lp, hd);
	Position testPosition("TestPosition1", 10, test1, 100, PositionType::LONG);

	test_portfolio.addPosition(testPosition);
	test_risk_engine.analysePortfolio(test_portfolio, TimeFrame::DAILY);

}  