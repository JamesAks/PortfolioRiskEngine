#include "../include/PortfolioManager.hpp"



// ----- Private -----

void PortfolioManager::displayPortfolioReport(PortfolioRiskReport report) const {

	printf("Portfolio ID: %s \n", report.ID.c_str());
	printf("Total Number of Positions : %zu \n\n", report.breakdowns.size());
	printf("Total Return: %f \n", report.total_return);
	printf("Expected Return: %f \n", report.expectedReturn);
	printf("Volatility: %f \n", report.volatitilty);

	printf("----- Positions Breakdown ----- \n\n");


	for (int i = 0; i < report.breakdowns.size(); i++) {

		printf("--- Position #%d --- \n\n", i + 1);
		displayPositionReport(report.breakdowns[i]);
		printf("\n");
	} 

	printf(" ----- Covariance Matrix ------ \n\n");
	std::cout << report.cov_matrix.matrixData() << "\n\n";
}


void PortfolioManager::displayPositionReport(PositionRiskReport report) const {

	const char* port_ID = report.position_ID.c_str();
	const char* asset_ID = report.asset_ID.c_str();


	printf("Position ID: %s \n", port_ID);
	printf("Underlying asset:  %s \n", asset_ID);
	printf("%s Current Market Value: %f \n", asset_ID, report.market_value);
	printf("Price Bought At: %f \n", report.price_bought_at);
	printf("Quantity Bought: %d \n", report.quantity);
	printf("Total Market Value: %f \n", report.total_market_value);
	printf("Initial Investment: %f \n", report.initial_investment);
	printf("Unrealized gains: %f \n", report.unrealised_gain);
	printf("%s Volatility: %g \n", asset_ID, report.volatility);
	printf("%s Current Market Value: %f \n", asset_ID, report.market_value);
	printf("%s Expected Return: %f \n", asset_ID, report.expected_return);
}


std::string PortfolioManager::avApiKey() const {

	const char* API_KEY = getenv("ALPHA_VANTAGE_API_KEY");
	return API_KEY;
}

// ----- Public -----

PortfolioManager::PortfolioManager() : adp(avApiKey()), market_data_manager{}, risk_engine{} {}
		

void PortfolioManager::analysePortfolio(std::string portfolio_ID, TimeFrame tf) const {

	auto p = portfolios.find(portfolio_ID);
	if (p == portfolios.end()) {

		Logger::logError(" Portfolio\"" + portfolio_ID + "\" not found.");
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


void PortfolioManager::addPosition(std::string position_ID, size_t quantity, std::string asset_ID, double price_bought_at,
									PositionType position_type, std::string portfolio_ID){

	if (asset_ID.empty() || position_ID.empty()) {

		Logger::logError("Position not added. Portfolio and Asset IDs cannot be empty.");
		return;
	}

	auto port = portfolios.find(portfolio_ID);

	if (port == portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" not found.");
		return;
	}

	auto as = asset_store.find(asset_ID);
	if (as != asset_store.end()) {

		Logger::logInfo("Asset found in asset store. Using previously stored asset data to construct position...");
		Position p(position_ID, quantity, as->second, price_bought_at, position_type );
		portfolios.find(portfolio_ID)->second.addPosition(p);
		Logger::logInfo("Added position \"" + position_ID + "\" to \"" + portfolio_ID + "\".");

		return;
	}

	Logger::logInfo("Atempting to fetch historic data for \"" + asset_ID + "\" and add to data store.");

	auto historicResult = market_data_manager.addHistoricalData(asset_ID,);//addMarketData(asset_ID);
	if (historicResult != RequestError::NONE) {

		Logger::logError("Asset not added. Error with fetching market data for \"" + asset_ID + "\".");
		return;
	}

	std::shared_ptr<Asset> asset = std::make_shared<Asset>(asset_ID, market_data_manager.currentPrice(asset_ID));

	asset_store.emplace(asset_ID, asset);
	Logger::logInfo("Added \"" + asset_ID + "\" to asset store.");

	Position p(position_ID, quantity, asset, price_bought_at, position_type);

	portfolios.find(portfolio_ID)->second.addPosition(p);
	Logger::logInfo("Added position \"" + position_ID + "\" to \"" + portfolio_ID + "\".");
	return;
}


void PortfolioManager::removePosition(std::string portfolio_ID, std::string position_ID){

	if (portfolio_ID.empty() || position_ID.empty()) {

		Logger::logError("Could not remove position. Portfolio and portfolio ID cannot be null.");
	}
	auto p = portfolios.find(portfolio_ID);
	if (p == portfolios.end()) {

		Logger::logError("Could not remove \"" + position_ID + "\". Portfolio \"" + portfolio_ID + "\" not found. Check spelling.");
		return;
	}

	std::map<std::string,Position> pos = p->second.viewPositions();

	if (pos.find(position_ID) == pos.end()) {
		 
		Logger::logError("Could not remove \"" + position_ID + "\". Position \"" + position_ID + "\" not found in portfolio \"" + portfolio_ID + "\". Check spelling.");
		return;
	}

	p->second.removePosition(position_ID);

	Logger::logInfo("Successfuly removed \"" + position_ID + "\" from \"" + portfolio_ID + "\".");
	return;
}

size_t PortfolioManager::portfolioSize(std::string portfolio_ID) const {

	auto port = portfolios.find(portfolio_ID);
	if (port == portfolios.end()) {

		Logger::logError("Could not find portfolio \"" + portfolio_ID + "\". Check spelling.");
	}

	return port->second.size();
}


size_t PortfolioManager::numbPortfolio() const { return portfolios.size(); }


void PortfolioManager::updateMarketData() { market_data_manager.update(); }

