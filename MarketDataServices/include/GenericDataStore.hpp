#ifndef MARKET_DATA_STORE_HPP
#define MARKET_DATA_STORE_HPP

#include "MarketDataStore.hpp"


#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
#include <thread>
#include <vector>




class HistoricData;
class TimeSeries;
class LatestPrice;

class GenericDataStore: public MarketDataStore{

	// Generic store for all the different market data needed for each asset.

	private:

		mutable std::shared_mutex mutex;
		std::shared_ptr<MarketDataProvider> equity_data_provider;
		std::map<std::string, std::shared_ptr<HistoricData>> historic_data_store;
		std::map<std::string, std::shared_ptr<LatestPrice>> latest_prices_store;

		void updateMarketData();
		void updateLoop(std::stop_token);
		
		size_t update_interval;
		size_t update_count;

		std::shared_ptr<std::map<std::string, MarketData>> market_data_snapshot;

		std::jthread update_thread;

	public:

		GenericDataStore(MarketDataProvider*);
		GenericDataStore(std::shared_ptr<MarketDataProvider>);
		GenericDataStore() = default;

		bool addMarketData(std::string) override;
		bool removeMarketData(std::string) override;
		bool changeDataProvider(std::shared_ptr<MarketDataProvider>);

		// Provides an immutable snapshot of the whole market data currently store in the the data store
		std::shared_ptr<const std::map<std::string, MarketData>> getMarketDataSnapshot() const override;

		std::shared_ptr<MarketDataProvider> viewDataProvider() const ;

		void update() override;
		void setUpdateIntervalMinute(size_t);

		size_t size() const;
};

#endif // !MARKET_DATA_STORE_HPP