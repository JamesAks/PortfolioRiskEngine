#include "../includes/MarketDataManager.hpp"



// ---- Private Members -----

RequestError MarketDataManager::updateLatestPrice(std::string symbol) {

	RequestResult res = adapter.latestPrice(symbol);

	if (res.requestError != RequestError::NONE) {

		Logger::logError("Could not update data for \"" + symbol + "\".");
		return res.requestError;
	}

	latestPrices[symbol] = res.price;
	Logger::logInfo("Successfully updated the latest price for \"" + symbol + "\".");

	return res.requestError;
}

// ---- Public Members -----

MarketDataManager::MarketDataManager(Adapter& adp) : adapter{ adp } {}


RequestError MarketDataManager::addMarketData(std::string symbol) {

	// Fetching and validating historic data.
	HistoricData hd;

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
	return RequestError::NONE;
}


const HistoricData& MarketDataManager::historicalData(std::string symbol) const {

	auto result = historicData.find(symbol);
	return result->second;
}


const TimeSeries& MarketDataManager::periodicData(std::string symbol, TimeFrame tf) const {

	auto pd = historicData.find(symbol);
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


const double& MarketDataManager::currentPrice(std::string symbol) const {

	return latestPrices.find(symbol)->second;
}


const std::map<std::string, HistoricData>& MarketDataManager::viewHistoricData() const { return historicData; }


const std::map<std::string, double>& MarketDataManager::viewLatestPrices() const { return latestPrices; }


std::vector<std::string> MarketDataManager::viewSymbols() const {
	
	std::vector<std::string> symbols;
	for (std::pair d : historicData) {

		symbols.push_back(d.first);
	}

	return symbols;
}


void MarketDataManager::update() {

	Logger::logInfo("Updating Market Data Store.....");

	int updated = 0;
	for (std::pair<std::string, HistoricData> entry : historicData) {
		std::string symbol = entry.first;

		HistoricData hd = historicData.find(symbol)->second;

		RequestResult daily = adapter.periodicData(symbol, TimeFrame::DAILY);
		if (daily.requestError != RequestError::NONE) {

			Logger::logError("Could not update the daily data for \"" + symbol + "\". Aborting Update...");
			continue;
		}

		RequestResult weekly = adapter.periodicData(symbol, TimeFrame::WEEKLY);
		if (weekly.requestError != RequestError::NONE) {

			Logger::logError("Could not update the weekly data for \"" + symbol + "\". Aborting Update...");
			continue;
		}

		RequestResult monthly = adapter.periodicData(symbol, TimeFrame::MONTHLY);
		if (monthly.requestError != RequestError::NONE) {

			Logger::logError("Could not update the monthly data for \"" + symbol + "\". Aborting Update...");
			continue;
		}

		hd.daily = daily.historicData.value();
		hd.weekly = weekly.historicData.value();
		hd.monthly = monthly.historicData.value();

		RequestResult lp = adapter.latestPrice(symbol);

		if (lp.requestError != RequestError::NONE) {

			Logger::logError("Could not update latest price of \"" + symbol + "\". Aborting Update...");
			continue;
		};

		historicData[symbol] = hd;
		latestPrices[symbol] = lp.price;

		Logger::logInfo("Successfully updated historic data for \"" + symbol + "\".");
		updated++;
	}

	size_t size = historicData.size();
	if (updated == size) {

		Logger::logInfo("Data store fully updated: " + std::to_string(updated) + "/" + std::to_string(size) + " Updated.");
		return;
	}

	Logger::logWarning("Data store not fully updated: " + std::to_string(updated) + "/" + std::to_string(size) + " Updated.");
}