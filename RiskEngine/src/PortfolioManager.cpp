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
		

void PortfolioManager::analysePortfolio(std::string portfolio_ID, TimeFrame tf) const {

	auto p = portfolios.find(portfolio_ID);
	if (p == portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" not found.");
		return;
	}

 	PortfolioRiskReport report = risk_engine.analysePortfolio(p->second, tf);
	displayPortfolioReport(report);

}


void PortfolioManager::createPortfolio(std::string portfolio_ID) {

	if (portfolios.find(portfolio_ID) != portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" already exist.");
		return;
	}

	Portfolio pf(portfolio_ID);
	portfolios.emplace(portfolio_ID, pf);
	Logger::logInfo("Created portfolio \"" + portfolio_ID + "\".");
}


void PortfolioManager::removePortfolio(std::string portfolio_ID) {

	if (portfolios.find(portfolio_ID) == portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" not found.");
	}


	portfolios.erase(portfolio_ID);
	Logger::logInfo("Deleted portfolio \"" + portfolio_ID + "\".");
	
}


void PortfolioManager::addPosition(size_t quant, std::string symbol, std::string portfolio_ID) {

	if (symbol.empty()) {

		Logger::logError("Asset not added.Symbol cannot be empty.");
		return;
	}

	auto port = portfolios.find(portfolio_ID);

	if (port == portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" not found.");
		return;
	}

	auto as = asset_store.find(symbol);
	if (as != asset_store.end()) {

		Logger::logInfo("Asset found in asset store. Using previously stored asset data to construct position...");
		Position p{ quant, as->second };
		portfolios.find(portfolio_ID)->second.addPosition(p);
		Logger::logInfo("Added position \"" + symbol + "\" to \"" + portfolio_ID + "\".");

		return;
	}

	Logger::logInfo("Atempting to fetch historic data for \"" + symbol + "\" and add to data store.");

	auto historicResult = market_data_manager.addMarketData(symbol);
	if (historicResult != RequestError::NONE) {

		Logger::logError("Asset not added. Error with fetching market data for \"" + symbol + "\".");
		return;
	}

	Position p{

		quant,
		std::make_shared<Asset>(symbol, market_data_manager.currentPrice(symbol))
	};

	asset_store.emplace(symbol, p.asset);
	Logger::logInfo("Added \"" + symbol + "\" to asset store.");

	portfolios.find(portfolio_ID)->second.addPosition(p);
	Logger::logInfo("Added position \"" + symbol + "\" to \"" + portfolio_ID + "\".");
	return;
}


void PortfolioManager::removePosition(std::string portfolio_ID, std::string symbol){



	auto p = portfolios.find(portfolio_ID);
	if (p == portfolios.end()) {

		Logger::logError("Could not remove \"" + symbol + "\". Portfolio \"" + portfolio_ID + "\" not found. Check spelling.");
		return;
	}

	std::map<std::string,Position> pos = p->second.viewPositions();

	if (pos.find(symbol) == pos.end()) {
		 
		Logger::logError("Could not remove \"" + symbol + "\". Position \"" + symbol + "\" not found in portfolio \"" + portfolio_ID + "\". Check spelling.");
		return;
	}

	p->second.removePosition(symbol);

	Logger::logInfo("Successfuly removed \"" + symbol + "\" from \"" + portfolio_ID + "\".");
	return;
}


void PortfolioManager::updateMarketData() { market_data_manager.update(); }

