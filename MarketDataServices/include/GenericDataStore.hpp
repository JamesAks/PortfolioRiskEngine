#ifndef MARKET_DATA_STORE_HPP
#define MARKET_DATA_STORE_HPP

#include <map>
#include <string>
#include "Core.hpp"
#include "MarketDataStore.hpp"


class GenericDataStore: MarketDataStore {

	private:

		std::map<std::string, HistoricData> historic_data_store;
		std::map<std::string, double> latest_price_store;

	public:
		GenericDataStore();
		void addHistoricalData(std::string, HistoricData);
		void addLatestPrice(std::string, double);
		void removeMarketData(std::string);

		const HistoricData& historicalData(std::string) const;
		const TimeSeries& periodicData(std::string, TimeFrame) const;
		const double& currentPrice(std::string) const;

		const std::map<std::string, HistoricData>& viewHistoricData() const;
		const std::map<std::string, double>& viewLatestPrices() const;
		std::vector<std::string> viewSymbols() const;

		void updateHistoricData(std::string, HistoricData);
		void updateLatestPrice(std::string, double);
		
};

#endif // !MARKET_DATA_STORE_HPP