#ifndef MARKET_DATA_STORE_HPP
#define MARKET_DATA_STORE_HPP

#include "MarketDataStore.hpp"


#include <map>
#include <memory>
#include <shared_mutex>
#include <string>
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

		void updateHistoricData() override;
		void updateLatestPrices() override;

	public:

		GenericDataStore(MarketDataProvider*);
		GenericDataStore(std::shared_ptr<MarketDataProvider>);
		GenericDataStore() = default;

		bool addMarketData(std::string) override;
		bool removeMarketData(std::string) override;
		bool changeDataProvider(std::shared_ptr<MarketDataProvider>);

		std::optional<HistoricData> getHistoricData(std::string) const override;
		std::optional<LatestPrice> getLatestPrice(std::string) const override;
		std::optional<MarketData> getMarketData(std::string) const override;

		std::vector<std::optional<HistoricData>> getHistoricDatas(const std::vector<std::string>&) const override;
		std::vector <std::optional<LatestPrice>> getLatestPrices(const std::vector<std::string>&) const override;
		std::vector <std::optional<MarketData>> getMarketDatas(const std::vector<std::string>&) const override;

		// Provides an immutable snapshot of the whole market data currently store in the the data store
		std::shared_ptr<const std::map<std::string, MarketData>> getMarketDataSnapshot() const override;

		std::optional<TimeSeries> periodicData(std::string, TimeFrame) const override;
		
		std::map<std::string, std::shared_ptr<HistoricData>> viewAllHistoricData() const override;
		std::map<std::string, std::shared_ptr<LatestPrice>> viewAllLatestPrices() const override;
		std::vector<std::string> viewSymbols() const override;

		std::shared_ptr<MarketDataProvider> viewDataProvider() const ;
		void update() override;

		size_t size() const;
};

#endif // !MARKET_DATA_STORE_HPP