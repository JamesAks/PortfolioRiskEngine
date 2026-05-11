#include "../include/PortfolioManager.hpp"



// ----- Private -----

void PortfolioManager::displayPortfolioReport(PortfolioRiskReport report) const {

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
	std::cout << report.cov_matrix.matrixData() << "\n\n";
}


void PortfolioManager::displayAssetReport(AssetRiskReport report) const {

	printf("Asset ID : %s \n", report.ID.c_str());
	printf("Volatility : %g \n", report.volatility);
	printf("Current Market Value : %f \n", report.market_value);
	printf("Expected Return : %f \n", report.expected_return);
}


std::string PortfolioManager::avApiKey() const {

	const char* API_KEY = getenv("ALPHA_VANTAGE_API_KEY");
	return API_KEY;
}

// ----- Public -----

PortfolioManager::PortfolioManager() : adp(avApiKey()), market_data_manager(adp), risk_engine(market_data_manager) {}
		

void PortfolioManager::analysePortfolio(std::string port, TimeFrame tf) const {

	auto p = portfolios.find(port);
	if (p == portfolios.end()) {

		printf("There is no portfolio called \"%s\" \n", port.c_str());
		return;
	}

 	PortfolioRiskReport report = risk_engine.analysePortfolio(p->second, tf);
	displayPortfolioReport(report);
}


void PortfolioManager::createPortfolio(std::string portfolio_ID) {

	Portfolio pf(portfolio_ID);
	portfolios.emplace(portfolio_ID, pf);
}


void PortfolioManager::removePortfolio(std::string portfolio_ID) {

	if (portfolios.find(portfolio_ID) == portfolios.end()) {

		printf("There is no Portfolio called \"%s\". \n", portfolio_ID.c_str());
	}
	else {

		portfolios.erase(portfolio_ID);
		printf("Portfolio successfully deleted. \n");
	}
}


void PortfolioManager::addPosition(size_t quant, std::string symbol, std::string portfolio_ID) {

	if (symbol.empty()) {

		printf("Asset not added. Symbol cannot be empty. \n");
		return;
	}

	auto historicResult = market_data_manager.addHistoricData(symbol);

	if (historicResult.resquest_error != RequestError::NONE) {

		printf("Asset not added. Error with fetching historic market data for \"%s\". Adapter error message: \n %s \n", symbol.c_str(), historicResult.error_message.c_str());
		return;
	}

	auto latestResult = market_data_manager.addLatestPrice(symbol);

	if (historicResult.resquest_error != RequestError::NONE) {

		printf("Asset not added. Error with fetching latest market quote for \"%s\", Adapter error message: \n %s \n", symbol.c_str(), latestResult.error_message.c_str());
		return;

	}

	auto as = asset_store.find(symbol);
	Position p;

	p.quantity = quant;

	if (as == asset_store.end()) {
		
		p.asset = std::make_shared<Asset>(symbol, market_data_manager.currentPrice(symbol));
		asset_store.emplace(symbol, p.asset);
	}
	else {

		p.asset = as->second;
	}

	portfolios.find(portfolio_ID)->second.addPosition(p);
}


void PortfolioManager::removePosition(std::string portfolio_ID, std::string symbol){

	auto p = portfolios.find(portfolio_ID);
	if (p == portfolios.end()) {

		printf("Portfolio \"%s\"not found. \n", portfolio_ID.c_str());
		return;
	}

	for (Position pos : p->second.viewPositions()) {

		if (pos.asset->symbol() == symbol) {

			p->second.removePosition(symbol);
			return;
		}
	}

	printf("Position \"%s\" not found in portfolio. \n", symbol.c_str());
	return;
}


void PortfolioManager::updateMarketData() { 

	for (std::pair<std::string,HistoricData> hd : market_data_manager.viewHistoricData()) {

		auto result = market_data_manager.updateHistoricData(hd.first);
		if (result.resquest_error != RequestError::NONE){

			printf("Could not update data for \"%s\". Adapter error message: \n \"%s\". \n", hd.first.c_str(), result.error_message.c_str());
			return;
		}
		
		printf("Updated Historic Data for \"%s\". \n", hd.first.c_str());

		auto lr = market_data_manager.updateLatestData(hd.first);
		if (lr.resquest_error != RequestError::NONE) {

			printf("Could not update data for \"%s\". Adapter error message: \n \"%s\". \n", hd.first.c_str(), lr.error_message.c_str());
			return;
		}
	};   
}

