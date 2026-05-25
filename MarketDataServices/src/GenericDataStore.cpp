#include "../include/GenericDataStore.hpp"
#include "Logger.hpp"



// ---- Private Members -----


// ---- Public Members -----

GenericDataStore::GenericDataStore() : historic_data_store{ {} }, latest_price_store{ {} } {}


void GenericDataStore::addHistoricalData(std::string symbol, HistoricData hist_data) {

	// Fetching and validating historic data.

	auto hd = historic_data_store.find(symbol);
	if (hd != historic_data_store.end()) {

		Logger::logError("Data for \"" + symbol + "\" is already stored in the data store. Try updateHistoricalData() method instead.");
		return;
	}

	historic_data_store.emplace(symbol, hist_data);

	Logger::logInfo("Added historical data for \"" + symbol + "\" to data store.");

	return;

/*	HistoricData hd;

	RequestResult daily = adapter.periodicData(symbol, TimeFrame::DAILY);
	if (daily.requestError != RequestError::NONE) {

		Logger::logError("Failed to fetch market data. Could not add \"" + symbol + "\" historic daily data to data store.");
		return daily.requestError;
	}

	RequestResult weekly = adapter.periodicData(symbol, TimeFrame::WEEKLY);
	if (weekly.requestError != RequestError::NONE) {

		Logger::logError("Failed to fetch market data. Could not add \"" + symbol + "\" historic weekly data to data store.");
		return weekly.requestError;
	}  


	RequestResult monthly = adapter.periodicData(symbol, TimeFrame::MONTHLY);
	if (monthly.requestError != RequestError::NONE) {

		Logger::logError("Failed to fetch market data. Could not add \"" + symbol + "\" historic monthly data to data store.");
		return monthly.requestError ;
	}

	hd.daily = daily.historicData.value();
	hd.weekly = weekly.historicData.value();
	hd.monthly = monthly.historicData.value();

	// Fetching and validating latest price.
	RequestResult lp = adapter.latestPrice(symbol);

	if (lp.requestError != RequestError::NONE) {

		Logger::logError("Failed to fetch market data. Could not add \"" + symbol + "\" latest price to data store.");
		return lp.requestError;
	};

	historicData.emplace(symbol, hd);
	latestPrices.emplace(symbol, lp.price);
	
	Logger::logInfo("Added  \"" + symbol + "\" market data to the data store.");
	return RequestError::NONE;*/
}


void GenericDataStore::addLatestPrice(std::string symbol, double price) {

	auto lp = latest_price_store.find(symbol);
	if (lp != latest_price_store.end()) {

		Logger::logError("Data for \"" + symbol + "\" is already stored in the data store. Try updateLatestlPrice() method instead.");
		return;
	}

	latest_price_store.emplace(symbol, price);
	Logger::logInfo("Added latest price for \"" + symbol + "\" to data store.");

	return;
}


void GenericDataStore::removeMarketData(std::string symbol) {

	auto hd = historic_data_store.find(symbol);
	auto lp = latest_price_store.find(symbol);

	if (hd == historic_data_store.end() || lp == latest_price_store.end()) {

		Logger::logError("Could not find data entry for \"" + symbol + "\" inside data store.");

	}

	latest_price_store.erase(symbol);
	historic_data_store.erase(symbol);

	Logger::logInfo("Removed data for \"" + symbol + "\".");
	return;
}


const HistoricData& GenericDataStore::historicalData(std::string symbol) const {

	auto result = historic_data_store.find(symbol);
	if (result == historic_data_store.end()) {

		Logger::logError("Could not find data for \"" + symbol + "\" inside data store.");
		return {};
	}

	return result->second;
}


const TimeSeries& GenericDataStore::periodicData(std::string symbol, TimeFrame tf) const {

	auto pd = historic_data_store.find(symbol);
	switch (tf)
	{
	case TimeFrame::DAILY:

		return pd->second.daily;
		break;

	case TimeFrame::WEEKLY:

		return pd->second.weekly;
		break;

	case TimeFrame::MONTHLY:

		return pd->second.monthly;
		break;

	default:

		throw std::runtime_error("Missing TimeFrame.");
	}
}


const double& GenericDataStore::currentPrice(std::string symbol) const {

	return latest_price_store.find(symbol)->second;
}


const std::map<std::string, HistoricData>& GenericDataStore::viewHistoricData() const { return historic_data_store; }


const std::map<std::string, double>& GenericDataStore::viewLatestPrices() const { return latest_price_store; }


std::vector<std::string> GenericDataStore::viewSymbols() const {
	
	std::vector<std::string> symbols;
	for (std::pair d : historic_data_store) {

		symbols.push_back(d.first);
	}

	return symbols;
}


void GenericDataStore::updateHistoricData(std::string symbol, HistoricData hist_data) {

	auto hd = historic_data_store.find(symbol);
	if (hd == historic_data_store.end()) {

		Logger::logError("Could not find \"" + symbol + "\" inside data store.");
		return;
	}

	historic_data_store[symbol] = hist_data;
	Logger::logInfo("Updated historical data data for \"" + symbol + "\".");
	return;
}


void GenericDataStore::updateLatestPrice(std::string symbol, double price) {

	auto lp = latest_price_store.find(symbol);
	if (lp == latest_price_store.end()) {

		Logger::logError("Could not find \"" + symbol + "\" inside data store.");
		return;
	}

	latest_price_store[symbol] = price;
	Logger::logInfo("Updated latest price for \"" + symbol + "\".");
	return;
}
