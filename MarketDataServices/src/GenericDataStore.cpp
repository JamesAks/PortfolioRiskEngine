#include "GenericDataStore.hpp"
#include "MarketData.hpp"
#include "Logger.hpp"
#include "TimeSeries.hpp"

#include <functional>
#include <chrono>


// ---- Private Members -----

void GenericDataStore::updateMarketData(){

	
	if (market_data_snapshot->size() == 0) { 

		Logger::logInfo("Market data not updated as the data store is empty. ");
		return;
	}

	Logger::logInfo("Updating market data...");

	size_t updated = 0;

	for (auto& [name, market_data] : *market_data_snapshot) {

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

		auto lp = equity_data_provider->latestPrice(name);
		if (lp.request_error != RequestError::NONE) {

			Logger::logWarning("Could not update latest price for \"" + name + "\".");
			continue;
		}

		// Update the current historic data and log.
		// Unique lock so nothing reads or rights to the historic data while writing the new data. 

		HistoricData historic_data{

			std::move(daily.time_series.value()),
			std::move(weekly.time_series.value()),
			std::move(monthly.time_series.value())
		};

		LatestPrice price{ lp.price };

		market_data = MarketData(historic_data, price);

		Logger::logInfo("Updated historic data for \"" + name + "\".");
		updated++;
	}

	Logger::logInfo("Updating finished. " + std::to_string(updated) + " out of " + std::to_string(latest_prices_store.size()) + " entries have been updated.");

	if (updated != market_data_snapshot->size()) {

		Logger::logWarning("Could not update all entries. Check logs.");
		return;
	}

	Logger::logInfo("Updated all market data entries .");
}


void GenericDataStore::updateLoop(std::stop_token stop_token) {

	while (!stop_token.stop_requested()) {

		update();

		int interval;
		
		{
			std::unique_lock lock{ mutex };
			interval = update_interval;
			update_count++;
		}

		std::this_thread::sleep_for(std::chrono::minutes(interval));
	}
}

// ---- Public Members -----

GenericDataStore::GenericDataStore(MarketDataProvider* mdp) : equity_data_provider{ mdp }, market_data_snapshot{std::make_shared<std::map<std::string, MarketData>>()}, update_interval{1}, update_count{0}, update_thread{std::bind_front(&GenericDataStore::updateLoop, this)} {}


GenericDataStore::GenericDataStore(std::shared_ptr<MarketDataProvider> mdp) : equity_data_provider{ mdp }, market_data_snapshot{ std::make_shared<std::map<std::string, MarketData>>() }, update_interval{ 1 }, update_count{ 0 }, update_thread{ std::bind_front(&GenericDataStore::updateLoop, this) } {}


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

		auto historic_data = std::make_shared<HistoricData>(

			std::move(daily.time_series.value()),
			std::move(weekly.time_series.value()),
			std::move(monthly.time_series.value())
		);

		auto latest_p = std::make_shared<LatestPrice>(latest_price.price);

		historic_data_store.emplace(symbol, historic_data);

		latest_prices_store.emplace(symbol, latest_p);

		market_data_snapshot->emplace(symbol, MarketData(*historic_data, *latest_p));
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


std::shared_ptr<const std::map<std::string, MarketData>> GenericDataStore::getMarketDataSnapshot() const {

	std::shared_lock lock{ mutex };
	return market_data_snapshot;
}


std::shared_ptr<MarketDataProvider> GenericDataStore::viewDataProvider() const { return equity_data_provider; }


size_t GenericDataStore::size() const { return historic_data_store.size(); }


void GenericDataStore::update() {

	{
		std::unique_lock lock{ mutex };
		updateMarketData();
	}
}


void GenericDataStore::setUpdateIntervalMinute(size_t minutes) { update_interval = minutes; }