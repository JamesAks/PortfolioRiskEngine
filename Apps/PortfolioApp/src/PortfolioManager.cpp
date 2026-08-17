#include "AlphaVantageProvider.hpp"
#include "Equities\Stock.hpp"
#include "EfficientFrontier.hpp"
#include "GenericDataStore.hpp"
#include "Logger.hpp"
#include "MarketData.hpp"
#include "Portfolio.hpp"
#include "PortfolioManager.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"



// ----- Private -----

std::shared_ptr<Stock> PortfolioManager::createStock(const std::string& asset_id) const {
	
	if (market_data_store->viewAllHistoricData().find(asset_id)->second == nullptr) {

		if (!market_data_store->addMarketData(asset_id)) {

			return nullptr;
		}
	}

	auto stock = std::make_shared<Stock>(asset_id);

	return stock;
}


// ----- Public -----

PortfolioReport PortfolioManager::analysePortfolio(TimeFrame tf) const {

	RiskEngine risk_engine{ market_data_store->getMarketDataSnapshot() };

	return risk_engine.analysePortfolio(*current_portfolio, tf);
}

PositionReport PortfolioManager::analysePosition(QString position_id, TimeFrame tf) const {

	const Position& position = current_portfolio->viewPosition(position_id.toStdString());

	RiskEngine risk_engine{ market_data_store->getMarketDataSnapshot() };

	return risk_engine.analysePosition(position, tf);
}


void PortfolioManager::addPortfolio(std::string portfolio_id) {

	auto portfolio = std::make_shared<Portfolio>(portfolio_id);
	portfolios.emplace(portfolio_id, portfolio);
}

void PortfolioManager::removePortfolio(const QString& portfolio_id) {

	if (portfolios.find(portfolio_id.toStdString()) != portfolios.end()) { portfolios.erase(portfolio_id.toStdString()); }
}


Portfolio* PortfolioManager::currentPortfolio() const { return current_portfolio; }


void PortfolioManager::setCurrentPortfolio(QString portfolio_id) {

	auto portfolio = portfolios.find(portfolio_id.toStdString());
	if (portfolio == portfolios.end()) {
		return;
	}

	current_portfolio = portfolio->second.get();
}


void PortfolioManager::createPortfolio(std::string portfolio_ID) {

	if (portfolios.find(portfolio_ID) != portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" already exist.");
		return;
	}

	portfolios.emplace(portfolio_ID, std::make_shared<Portfolio>(portfolio_ID));
	Logger::logInfo("Created portfolio \"" + portfolio_ID + "\".");
}


void PortfolioManager::removePortfolio(std::string portfolio_ID) {

	if (portfolios.find(portfolio_ID) == portfolios.end()) {

		Logger::logError("Portfolio \"" + portfolio_ID + "\" not found.");
	}

	portfolios.erase(portfolio_ID);
	Logger::logInfo("Deleted portfolio \"" + portfolio_ID + "\".");
	
}


double PortfolioManager::calculatePortfolioRisk(TimeFrame tf) const {

	if (current_portfolio->size() == 0) { return 0; }

	RiskEngine risk_engine{ market_data_store->getMarketDataSnapshot() };

	return risk_engine.portfolioVolatility(*current_portfolio, tf);
}


double PortfolioManager::calculateSharpeRatio(TimeFrame tf) const {

	if (current_portfolio->size() == 0) { return 0; }

	RiskEngine risk_engine{ market_data_store->getMarketDataSnapshot() };

	return risk_engine.portfolioSharpeRatio(*current_portfolio, tf,0);
}


EfficientFrontier PortfolioManager::calculateEfficientFrontier(TimeFrame tf) const {

	RiskEngine risk_engine{ market_data_store->getMarketDataSnapshot() };
	return *risk_engine.calculateEfficientFrontier(*current_portfolio, tf);
}


void PortfolioManager::addPosition( std::string position_id, size_t quantity, std::shared_ptr<Asset> asset, double price_bought_at,
									 PositionType position_type){

	if (asset == nullptr) { return; }
	current_portfolio->addPosition(std::make_shared<Position>(position_id, quantity, asset, price_bought_at, position_type));

}


void PortfolioManager::removePosition(std::string position_id){current_portfolio->removePosition(position_id);}


size_t PortfolioManager::portfolioSize() const {

	return current_portfolio->size();
}


size_t PortfolioManager::numbPortfolio() const { return portfolios.size(); }


const std::map<std::string, std::shared_ptr<Portfolio>>& PortfolioManager::viewPortfolios() const {

	return portfolios;
}


std::vector<std::string> PortfolioManager::viewPortfolioIDs() const {

	std::vector<std::string> names;
	names.reserve(numbPortfolio());

	for (const auto& [name, port] : portfolios) {

		names.push_back(name);
	}

	return names;
}


const Position& PortfolioManager::viewPosition(const QString& position_id) const {

	return current_portfolio->viewPosition(position_id.toStdString());
}

const MarketDataStore& PortfolioManager::viewDataStore() const { return *market_data_store; }

void PortfolioManager::registerMarketDataStore(MarketDataStore* mds) { market_data_store = mds; }

std::shared_ptr<const std::map<std::string, MarketData>>& PortfolioManager::viewSnapshot() const {

	auto result = market_data_store->getMarketDataSnapshot();

	return result;
}