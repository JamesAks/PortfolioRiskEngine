#include "Asset.hpp" 
#include "RiskEngine.hpp"
#include "GenericDataStore.hpp"
#include "CSVDataProvider.hpp"
#include "Portfolio.hpp"
#include "Logger.hpp"

#include <string>

#include <iostream>

static std::string DIR_PATH = "../../../../../../DataLoader/TestMarketData";

static HistoricData getHistoricData(std::string symbol, CSVDataProvider& market_data_provider) {

	auto daily = market_data_provider.periodicData(symbol, TimeFrame::DAILY, 100);
	if (daily.requestError != RequestError::NONE) {

		Logger::logError("Could not get daily data price for \"APPL\".");
		return {};
	}

	auto weekly = market_data_provider.periodicData(symbol, TimeFrame::WEEKLY, 100);
	if (weekly.requestError != RequestError::NONE) {

		Logger::logError("Could not get weekly data price for \"APPL\".");
		return {};
	}
	auto monthly = market_data_provider.periodicData(symbol, TimeFrame::MONTHLY, 100);
	if (monthly.requestError != RequestError::NONE) {

		Logger::logError("Could not get monthly data price for \"APPL\".");
		return {};
	}

	return { daily.historicData.value(), weekly.historicData.value(), monthly.historicData.value() };
}


double getLatestPrice(std::string symbol, CSVDataProvider& market_data_provider) {

	auto lp = market_data_provider.latestPrice(symbol);

	if (lp.requestError != RequestError::NONE) {

		Logger::logError("Could not get latest price for \"APPL\".");
		return NULL;
	}

	return lp.price;
}

static void displayPositionReport(PositionRiskReport report) {

	const char* port_ID = report.position_ID.c_str();
	const char* asset_ID = report.asset_ID.c_str();


	printf("Position ID: %s \n", port_ID);
	printf("Underlying asset:  %s \n", asset_ID);
	printf("%s Current Market Value: %f \n", asset_ID, report.market_value);
	printf("Price Bought At: %f \n", report.price_bought_at);
	printf("Quantity Bought: %zu \n", report.quantity);
	printf("Total Market Value: %f \n", report.total_market_value);
	printf("Initial Investment: %f \n", report.initial_investment);
	printf("Unrealized gains: %f \n", report.unrealised_gain);
	printf("%s Volatility: %g \n", asset_ID, report.volatility);
	printf("%s Current Market Value: %f \n", asset_ID, report.market_value);
	printf("%s Expected Return: %f \n", asset_ID, report.expected_return);
}

static void displayPortfolioReport(PortfolioRiskReport report) {

	printf("Portfolio ID: %s \n", report.ID.c_str());
	printf("Total Number of Positions : %zu \n\n", report.breakdowns.size());
	printf("Total Return: %f \n", report.total_return);
	printf("Expected Return: %f \n", report.expectedReturn);
	printf("Volatility: %f \n", report.volatitilty);
	printf("95%% Historical VaR: %f \n", report.historical_VaR);
	printf("95%% Historical Shortfall: %f \n", report.historical_shortfall);
	printf("95%% Parametric VaR: %f \n", report.parametric_VaR);
	printf("95%% Parametric Shortfall: %f \n", report.parametric_shortfall);
	printf("Sharpe Ratio: %f \n", report.sharpe_ratio);

	printf("----- Positions Breakdown ----- \n\n");

	for (int i = 0; i < report.breakdowns.size(); i++) {

		printf("--- Position #%d --- \n\n", i + 1);
		displayPositionReport(report.breakdowns[i]);
		printf("\n");
	}

	printf(" ----- Covariance Matrix ------ \n\n");
	std::cout << report.cov_matrix.matrixData() << "\n\n";
}


int main() {
	 
	GenericDataStore market_data_store;
	CSVDataProvider market_data_provider(DIR_PATH);
	RiskEngine test_risk_engine;
	Portfolio test_portfolio(std::string("TestPortfolio1"));

	auto aapl_data = getHistoricData("AAPL", market_data_provider);
	auto aapl_lp = getLatestPrice("AAPL", market_data_provider);

	auto ibm_data = getHistoricData("IBM", market_data_provider);
	auto ibm_lp = getLatestPrice("IBM", market_data_provider);


	market_data_store.addHistoricalData("AAPL", aapl_data);
	market_data_store.addLatestPrice("AAPL", aapl_lp);

	market_data_store.addHistoricalData("IBM", ibm_data);
	market_data_store.addLatestPrice("IBM", ibm_lp);

	auto aapl_hd = market_data_store.historicalData("AAPL");
	auto aapl_l = market_data_store.latestPrice("AAPL");

	auto ibm_hd = market_data_store.historicalData("IBM");
	auto ibm_l = market_data_store.latestPrice("IBM");

	auto test1 = std::make_shared<Asset>("AAPL", aapl_l, aapl_hd);
	Position test_position_1("TestPosition1", 10, test1, 100, PositionType::LONG);

	auto test2 = std::make_shared<Asset>("IBM", ibm_l, ibm_hd);
	Position test_position_2("TestPosition2", 10, test2, 100, PositionType::LONG);

	test_portfolio.addPosition(test_position_1);
	test_portfolio.addPosition(test_position_2);

	auto report = test_risk_engine.analysePortfolio(test_portfolio, TimeFrame::DAILY);
	auto returns = test_risk_engine.portfolioPeriodicReturns(test_portfolio, TimeFrame::DAILY, 100);


	std::cout << std::endl;

	displayPortfolioReport(report);


}  