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

	for (auto& [name, historic_data] : historic_data_store) {

		// If the market data provider can't provide any of the timeseries skip current entry and keep going.
		auto daily = equity_data_provider->periodicData(name, TimeFrame::DAILY, 100);
		if (daily.request_error != RequestError::NONE) { 

			Logger::logWarning("Could not update market data for \"" + name + "\".");
			continue;
		}

		auto weekly = equity_data_provider->periodicData(name, TimeFrame::WEEKLY, 100);
		if (weekly.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update market data for \"" + name + "\".");
			continue;
		}

		auto monthly = equity_data_provider->periodicData(name, TimeFrame::MONTHLY, 100);
		if (monthly.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update market data for \"" + name + "\".");
			continue;
		}

		// Update the current historic data and log.
		// Unique lock so nothing reads or rights to the historic data while writing the new data. 
		{ 
			std::unique_lock lock{ mutex };
			historic_data = std::make_shared<HistoricData>(

				std::move(daily.time_series.value()),
				std::move(weekly.time_series.value()),
				std::move(monthly.time_series.value())
			);
		}
		
		
		Logger::logInfo("Updated historic data for \"" + name + "\".");
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

	for (auto& [name, price] : latest_prices_store) {

		// If latest price not found then skip current entry and keep going.
		auto lp = equity_data_provider->latestPrice(name);
		if (lp.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update latest price for \"" + name + "\".");
			continue;
		}

		// Update latest price and log.
		// Unique lock so nothing reads or rights to the historic data while writing the new data. 
		{ 
			std::unique_lock lock{ mutex };
			price = std::make_shared<LatestPrice>(std::move(lp.price));
		}
		
		Logger::logInfo("Updated historic data for \"" + name + "\".");
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
	{
		std::unique_lock lock{ mutex };
		historic_data_store.emplace(symbol, std::make_shared<HistoricData>(

			std::move(daily.time_series.value()),
			std::move(weekly.time_series.value()),
			std::move(monthly.time_series.value())

		));

		latest_prices_store.emplace(symbol, std::make_shared<LatestPrice>(latest_price.price));
	}

	Logger::logInfo("Added market data for \"" + symbol + "\" to data store.");
	return true;
}


bool GenericDataStore::removeMarketData(std::string symbol) {

	{
		std::shared_lock lock{ mutex };
		if (historic_data_store.find(symbol) == historic_data_store.end() || latest_prices_store.find(symbol) == latest_prices_store.end()) {

			Logger::logError("Could not find data entry for \"" + symbol + "\" inside data store.");
			return false;
		}
	}
	
	{
		std::unique_lock lock{ mutex };
		latest_prices_store.erase(symbol);
		historic_data_store.erase(symbol);
	}


	Logger::logInfo("Removed market data for \"" + symbol + "\" from data store.");
	return true;
}

bool GenericDataStore::changeDataProvider(std::shared_ptr<MarketDataProvider> mdp) {

	if (mdp != nullptr) { return false; }

	equity_data_provider = mdp;
	Logger::logInfo("Changed market data provider.");
	return true;	
};


std::optional<HistoricData> GenericDataStore::getHistoricData(std::string symbol) const {

	std::optional<HistoricData> result;

	// The data could be deleted between finding the iterator and retrieving the object.
	{
		std::shared_lock lock{ mutex };

		auto iter = historic_data_store.find(symbol);

		if (iter != historic_data_store.end()) {

			result = *iter->second;
		}
		else { Logger::logError("Could not find historical data for \"" + symbol + "\" inside data store."); }
	}

	return result;
}

std::optional<LatestPrice> GenericDataStore::getLatestPrice(std::string symbol) const {

	std::optional<LatestPrice> result;

	{
		std::shared_lock lock{ mutex };

		auto iter = latest_prices_store.find(symbol);

		if (iter != latest_prices_store.end()) {

			result = *iter->second;
		}
		else { Logger::logError("Could not find latest price for \"" + symbol + "\" inside data store"); }
	}

	return result;
}

std::optional<MarketData> GenericDataStore::getMarketData(std::string symbol) const {

	auto hist = getHistoricData(symbol);
	if (!hist.has_value()) { return std::nullopt; }

	auto late = getLatestPrice(symbol);
	if (!late.has_value()) { return std::nullopt; }

	MarketData result{ hist.value(),late.value() };

	return result;
}



std::vector<std::optional<HistoricData>> GenericDataStore::getHistoricDatas(const std::vector<std::string>& symbols) const{

	std::vector<std::optional<HistoricData>> results;
	results.reserve(symbols.size());

	{
		std::shared_lock lock{ mutex };
		for (auto& symbol : symbols) {

			auto iter = historic_data_store.find(symbol);

			if (iter != historic_data_store.end()) {

				results.push_back(*iter->second);
			}
			else {

				Logger::logError("Could not find historical data for \"" + symbol + "\" inside data store.");
				results.push_back(std::nullopt);
			}
		}
	}

	return results;
}


std::vector <std::optional<LatestPrice>> GenericDataStore::getLatestPrices(const std::vector<std::string>& symbols) const{

	std::vector<std::optional<LatestPrice>> results;
	results.reserve(symbols.size());

	{
		std::shared_lock lock{ mutex };
		for (auto& symbol : symbols) {

			auto iter = latest_prices_store.find(symbol);

			if (iter != latest_prices_store.end()) {

				results.push_back(*iter->second);
			}
			else {

				Logger::logError("Could not find historical data for \"" + symbol + "\" inside data store.");
				results.push_back(std::nullopt);
			}
		}
	}

	return results;
}


std::vector <std::optional<MarketData>> GenericDataStore::getMarketDatas(const std::vector<std::string>& symbols) const{
	
	std::vector <std::optional<MarketData>> results;
	results.reserve(symbols.size());

	auto hist = getHistoricDatas(symbols);
	auto late = getLatestPrices(symbols);

	for (int i = 0; i < symbols.size(); i++) {

		MarketData temp{ hist[i].value(), late[i].value() };
		results.push_back(temp);
	}

	return results;
}

std::shared_ptr<const std::map<std::string, MarketData>> GenericDataStore::getMarketDataSnapshot() const {

	auto snapshot = std::make_shared<std::map<std::string, MarketData>>();

	{
		std::shared_lock lock{ mutex };

		for (auto [name, hist_data] : historic_data_store) {

			MarketData temp{ *hist_data, *latest_prices_store.find(name)->second };
			snapshot->emplace(name, temp);
		}
	}

	return snapshot;
}

std::optional<TimeSeries> GenericDataStore::periodicData(std::string asset_ID, TimeFrame tf) const {
	
	std::optional<TimeSeries> result;

	{
		std::shared_lock lock{ mutex };

		auto iter = historic_data_store.find(asset_ID);
		if (iter != historic_data_store.end()) {

			switch (tf)
			{
			case TimeFrame::DAILY:
				result = iter->second->dailyData();
				break;

			case TimeFrame::WEEKLY:
				result = iter->second->weeklyData();
				break;

			case TimeFrame::MONTHLY:
				result = iter->second->monthlyData();
				break;

			default:

				throw std::runtime_error("Missing TimeFrame.");
			}

		}
		else { Logger::logError("Could not find asset " + asset_ID + " in store."); }
	}

	return result;
}


std::map<std::string, std::shared_ptr<HistoricData>> GenericDataStore::viewAllHistoricData() const { return historic_data_store; }


std::map<std::string, std::shared_ptr<LatestPrice>> GenericDataStore::viewAllLatestPrices() const { return latest_prices_store; }


std::vector<std::string> GenericDataStore::viewSymbols() const {
	
	std::vector<std::string> symbols;
	for (std::pair d : historic_data_store) {

		symbols.push_back(d.first);
	}

	return symbols;
}


std::shared_ptr<MarketDataProvider> GenericDataStore::viewDataProvider() const { return equity_data_provider; }


size_t GenericDataStore::size() const { return historic_data_store.size(); }

void GenericDataStore::update() {

	updateHistoricData();
	updateLatestPrices();
}