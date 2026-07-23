#ifndef MARKET_DATA_STORE_HPP
#define MARKET_DATA_STORE_HPP

#include "MarketDataStore.hpp"


#include <map>
#include <memory>
#include <string>
#include <vector>



class HistoricData;
class TimeSeries;
class LatestPrice;

class GenericDataStore: MarketDataStore{

	// Generic store for all the different market data needed for each asset.

	private:


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
		
		const HistoricData& viewHistoricData(std::string) const override;
		const LatestPrice& viewLatestPrice(std::string) const override;
		const TimeSeries& periodicData(std::string, TimeFrame) const override;
		

		const std::map<std::string, std::shared_ptr<HistoricData>>& viewAllHistoricData() const override;
		const std::map<std::string, std::shared_ptr<LatestPrice>>& viewAllLatestPrices() const override;
		std::vector<std::string> viewSymbols() const override;

		std::shared_ptr<MarketDataProvider> viewDataProvider() const ;


		size_t size() const;
};

#endif // !MARKET_DATA_STORE_HPP