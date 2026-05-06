#include "../include/PortfolioManager.hpp"
#include <iostream>



// ----- Private -----

void PortfolioManager::displayPortfolioReport(PortfolioRiskReport report) {

	printf("Portfolio ID : %s \n", report.ID.c_str());
	printf("Total Return : %f \n", report.total_return);
	printf("Expected Return : %f \n", report.expectedReturn);
	printf("Volatility : %f \n", report.volatitilty);

	printf("----- Assets Breakdown ----- \n\n");
	printf("Total Number of Assets : %zu \n\n", report.breakdowns.size());

	for (int i = 0; i < report.breakdowns.size(); i++) {

		printf("Asset %d \n", i + 1);
		displayAssetReport(report.breakdowns[i]);
		printf("\n");
	} 

	printf(" ----- Covariance Matrix ------ \n\n");
	std::cout << report.cov_matrix.data() << "\n\n";
}


void PortfolioManager::displayAssetReport(AssetRiskReport report) {

	printf("Asset ID : %s \n", report.ID.c_str());
	printf("Volatility : %g \n", report.volatility);
	printf("Market Value : %f \n", report.market_value);
	printf("Expected Return : %f \n", report.expected_return);
}

std::string PortfolioManager::avApiKey() {

	const char* API_KEY = getenv("ALPHA_VANTAGE_API_KEY");
}

// ----- Public -----

PortfolioManager::PortfolioManager() : adp(avApiKey()), market_data_manager(adp), risk_engine(market_data_manager) {}
		

void PortfolioManager::analysePortfolio(std::string port, TimeFrame tf) {

	auto p = portfolios.find(port);
	if (p != portfolios.end()) {

		PortfolioRiskReport report = risk_engine.analysePortfolio(p->second, tf);
		displayPortfolioReport(report);
	}
 	printf("There is no portfolio called %s \n", port.c_str());
}


void PortfolioManager::createPortfolio(std::string portfolio_ID) {

	Portfolio pf(portfolio_ID);
	portfolios.emplace(portfolio_ID, pf);
}


void PortfolioManager::removePortfolio(std::string portfolio_ID) {

	if (portfolios.find(portfolio_ID) == portfolios.end()) {

		printf("There is no Portfolio called %s.", portfolio_ID.c_str());
	}
	else {

		portfolios.erase(portfolio_ID);
		printf("Portfolio successfully deleted");
	}
}


void PortfolioManager::addPosition(size_t quant, std::string symbol, std::string portfolio_ID) {

	market_data_manager.addHistoricData(symbol);
	market_data_manager.addLatestPrice(symbol);

	Asset* as = new Asset(symbol, market_data_manager.currentPrice(symbol));
		
	Position pos{quant,as};

	portfolios.find(portfolio_ID)->second.addPosition(pos);
}


void PortfolioManager::removePosition(std::string portfolio_ID, std::string symbol){

	portfolios.find(portfolio_ID)->second.removePosition(symbol);
}


void PortfolioManager::updateHistoricMarketData() { market_data_manager.updateHistoricData(); }

