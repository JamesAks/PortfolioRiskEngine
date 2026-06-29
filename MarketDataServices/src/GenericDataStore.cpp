#include "GenericDataStore.hpp"
#include "HistoricData.hpp"
#include "Logger.hpp"
#include "TimeSeries.hpp"
#include <stdexcept>


// ---- Private Members -----


// ---- Public Members -----

GenericDataStore::GenericDataStore() : historic_data_store{}, latest_price_store{} {}


void GenericDataStore::addHistoricalData(std::string symbol, TimeSeries& daily, TimeSeries& weekly, TimeSeries& monthly) {


	if (historic_data_store.find(symbol) != historic_data_store.end()) {

		Logger::logError("Data for \"" + symbol + "\" is already stored in the data store. Try updateHistoricalData() method instead.");
		return;
	}

	historic_data_store.emplace(symbol, std::make_shared<HistoricData>(daily, weekly, monthly));
	Logger::logInfo("Added historical data for \"" + symbol + "\" to data store.");
}


void GenericDataStore::addHistoricalData(std::string symbol, HistoricData hd) {

	if (historic_data_store.find(symbol) != historic_data_store.end()) {

		Logger::logError("Data for \"" + symbol + "\" is already stored in the data store. Try updateHistoricalData() method instead.");
		return;
	}

	historic_data_store.emplace(symbol, std::make_shared<HistoricData>(std::move(hd)));
	Logger::logInfo("Added historical data for \"" + symbol + "\" to data store.");
}


void GenericDataStore::addLatestPrice(std::string symbol, LatestPrice price) {

	if (latest_price_store.find(symbol) != latest_price_store.end()) {

		Logger::logError("Data for \"" + symbol + "\" is already stored in the data store. Try updateLatestlPrice() method instead.");
		return;
	}

	latest_price_store.emplace(symbol, std::make_shared<LatestPrice>(price));
	Logger::logInfo("Added latest price for \"" + symbol + "\" to data store.");

	return;
}


void GenericDataStore::removeMarketData(std::string symbol) {

	if (historic_data_store.find(symbol) == historic_data_store.end() || latest_price_store.find(symbol) == latest_price_store.end()) {

		Logger::logError("Could not find data entry for \"" + symbol + "\" inside data store.");

	}

	latest_price_store.erase(symbol);
	historic_data_store.erase(symbol);

	Logger::logInfo("Removed data for \"" + symbol + "\".");
	return;
}


const std::shared_ptr<HistoricData> GenericDataStore::historicalData(std::string symbol) const {

	auto result = historic_data_store.find(symbol);
	if (result == historic_data_store.end()) {

		Logger::logError("Could not find historical data for \"" + symbol + "\" inside data store.");
		return nullptr;
	}

	return result->second;
}

const std::shared_ptr<LatestPrice> GenericDataStore::latestPrice(std::string symbol) const {

	auto result = latest_price_store.find(symbol);
	if (result == latest_price_store.end()) {

		Logger::logError("Could not find latest price for \"" + symbol + "\" inside data store.");
		return nullptr;
	}

	return latest_price_store.find(symbol)->second;
}


const TimeSeries& GenericDataStore::periodicData(std::string asset_ID, TimeFrame tf) const {

	auto pd = historic_data_store.find(asset_ID);
	if (pd == historic_data_store.end()) {

		Logger::logError("Could not find asset " + asset_ID + " in store.");
		return {};
	}

	switch (tf)
	{
	case TimeFrame::DAILY:

		return pd->second->dailyData();
		break;

	case TimeFrame::WEEKLY:

		return pd->second->weeklyData();
		break;

	case TimeFrame::MONTHLY:

		return pd->second->monthlyData();
		break;

	default:

		throw std::runtime_error("Missing TimeFrame.");
	}
}


const std::map<std::string, std::shared_ptr<HistoricData>>& GenericDataStore::viewHistoricData() const { return historic_data_store; }


const std::map<std::string, std::shared_ptr<LatestPrice>>& GenericDataStore::viewLatestPrices() const { return latest_price_store; }


std::vector<std::string> GenericDataStore::viewSymbols() const {
	
	std::vector<std::string> symbols;
	for (std::pair d : historic_data_store) {

		symbols.push_back(d.first);
	}

	return symbols;
}


void GenericDataStore::updateHistoricData(std::string symbol, TimeSeries& daily, TimeSeries& weekly, TimeSeries& monthly) {

	auto hd = historic_data_store.find(symbol);
	if (hd == historic_data_store.end()) {

		Logger::logError("Could not find \"" + symbol + "\" inside data store.");
		return;
	}

	*hd->second = { daily,weekly,monthly };

	Logger::logInfo("Updated historical data data for \"" + symbol + "\".");
	return;
}


void GenericDataStore::updateLatestPrice(std::string symbol, LatestPrice price) {

	auto lp = latest_price_store.find(symbol);
	if (lp == latest_price_store.end()) {

		Logger::logError("Could not find \"" + symbol + "\" inside data store.");
		return;
	}

	*lp->second = price;
	Logger::logInfo("Updated latest price for \"" + symbol + "\".");
	return;
}
