#include "GenericDataStore.hpp"
#include "MarketData.hpp"
#include "Logger.hpp"
#include "TimeSeries.hpp"



struct DataStoreResult{

};

// ---- Private Members -----

void GenericDataStore::updateHistoricData() {

	Logger::logInfo("Updating historic data store...");

	size_t updated = 0;

	for (auto& historic_data : historic_data_store) {

		// If the market data provider can't provide any of the timeseries skip current entry and keep going.
		auto daily = equity_data_provider->periodicData(historic_data.first, TimeFrame::DAILY, 100);
		if (daily.request_error != RequestError::NONE) { 

			Logger::logWarning("Could not update market data for \"" + historic_data.first + "\".");
			continue;
		}

		auto weekly = equity_data_provider->periodicData(historic_data.first, TimeFrame::WEEKLY, 100);
		if (weekly.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update market data for \"" + historic_data.first + "\".");
			continue;
		}

		auto monthly = equity_data_provider->periodicData(historic_data.first, TimeFrame::MONTHLY, 100);
		if (monthly.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update market data for \"" + historic_data.first + "\".");
			continue;
		}

		// Update the current historic data and log.
		historic_data_store[historic_data.first] = std::make_shared<HistoricData>(

			std::move(daily.time_series.value()), 
			std::move(weekly.time_series.value()),
			std::move(monthly.time_series.value())
		);
		
		Logger::logInfo("Updated historic data for \"" + historic_data.first + "\".");
		updated++;
	}

	Logger::logInfo("Updating finished. " + std::to_string(updated) + "out of " + std::to_string(historic_data_store.size()) + " entries have been updated.");

	if (updated != historic_data_store.size()) {

		Logger::logWarning("Could not update all entries in historic data store. Check logs.");
		return;
	}

	Logger::logInfo("Updated all entries in historic data store.");
}


void GenericDataStore::updateLatestPrices() {

	Logger::logInfo("Updating latest prices store...");

	size_t updated = 0;

	for (auto& latest_price : latest_prices_store) {

		// If latest price not found then skip current entry and keep going.
		auto lp = equity_data_provider->latestPrice(latest_price.first);
		if (lp.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update latest price for \"" + latest_price.first + "\".");
			continue;
		}

		// Update latest price and log.
		latest_prices_store[latest_price.first] = std::make_shared<LatestPrice>(std::move(lp.price));
		Logger::logInfo("Updated historic data for \"" +latest_price.first + "\".");
		updated++;
	}

	Logger::logInfo("Updating finished. " + std::to_string(updated) + "out of " + std::to_string(latest_prices_store.size()) + " entries have been updated.");

	if (updated != latest_prices_store.size()) {

		Logger::logWarning("Could not update all entries in latest price store. Check logs.");
		return;
	}

	Logger::logInfo("Updated all entries in latest price store.");
}

// ---- Public Members -----

GenericDataStore::GenericDataStore(MarketDataProvider* mdp) : equity_data_provider{ mdp } {}


GenericDataStore::GenericDataStore(std::shared_ptr<MarketDataProvider> mdp) : equity_data_provider{ mdp } {}


bool GenericDataStore::addMarketData(std::string symbol) {

	// If the data is already in the store then there is no need to fetch data again.
	if (historic_data_store.find(symbol) != historic_data_store.end()) {

		Logger::logError("Data for \"" + symbol + "\" is already stored in the data store. Try updateHistoricalData() method instead.");
		return true;
	}

	// Get Historical data from data provider.
	auto daily = equity_data_provider->periodicData(symbol, TimeFrame::DAILY, 100);
	if (daily.request_error != RequestError::NONE) { return false; }

	auto weekly = equity_data_provider->periodicData(symbol, TimeFrame::WEEKLY, 100);
	if (weekly.request_error != RequestError::NONE) { return false; }

	auto monthly = equity_data_provider->periodicData(symbol, TimeFrame::MONTHLY, 100);
	if (monthly.request_error != RequestError::NONE) { return false; }




	// Get Latest Price from data provider.
	auto latest_price = equity_data_provider->latestPrice(symbol);
	if (latest_price.request_error != RequestError::NONE) { return false; }

	// Add data to the respective data stores.
	historic_data_store.emplace(symbol, std::make_shared<HistoricData>(

		std::move(daily.time_series.value()),
		std::move(weekly.time_series.value()),
		std::move(monthly.time_series.value())
	
	));

	latest_prices_store.emplace(symbol, std::make_shared<LatestPrice>(latest_price.price));

	Logger::logInfo("Added market data for \"" + symbol + "\" to data store.");
	return true;
}


bool GenericDataStore::removeMarketData(std::string symbol) {

	if (historic_data_store.find(symbol) == historic_data_store.end() || latest_prices_store.find(symbol) == latest_prices_store.end()) {

		Logger::logError("Could not find data entry for \"" + symbol + "\" inside data store.");
		return false;
	}

	latest_prices_store.erase(symbol);
	historic_data_store.erase(symbol);

	Logger::logInfo("Removed market data for \"" + symbol + "\" from data store.");
	return true;
}

bool GenericDataStore::changeDataProvider(std::shared_ptr<MarketDataProvider> mdp) {

	if (mdp != nullptr) { return false; }

	equity_data_provider = mdp;
	Logger::logInfo("Changed market data provider.");
	return true;	
};


std::shared_ptr<HistoricData> GenericDataStore::getHistoricData(std::string symbol) const {

	auto result = historic_data_store.find(symbol);
	if (result == historic_data_store.end()) {

		Logger::logError("Could not find historical data for \"" + symbol + "\" inside data store.");
		return nullptr;
	}

	return result->second;
}

std::shared_ptr<LatestPrice> GenericDataStore::getLatestPrice(std::string symbol) const {

	auto result = latest_prices_store.find(symbol);
	if (result == latest_prices_store.end()) {

		Logger::logError("Could not find latest price for \"" + symbol + "\" inside data store.");
		return nullptr;
	}

	return result->second;
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


const std::map<std::string, std::shared_ptr<HistoricData>>& GenericDataStore::viewAllHistoricData() const { return historic_data_store; }


const std::map<std::string, std::shared_ptr<LatestPrice>>& GenericDataStore::viewAllLatestPrices() const { return latest_prices_store; }


std::vector<std::string> GenericDataStore::viewSymbols() const {
	
	std::vector<std::string> symbols;
	for (std::pair d : historic_data_store) {

		symbols.push_back(d.first);
	}

	return symbols;
}


std::shared_ptr<MarketDataProvider> GenericDataStore::viewDataProvider() const { return equity_data_provider; }


size_t GenericDataStore::size() const { return historic_data_store.size(); }