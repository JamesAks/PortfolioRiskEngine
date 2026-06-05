#ifndef MARKET_DATA_STORE_HPP
#define MARKET_DATA_STORE_HPP

#include "MarketDataStore.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>



class HistoricData;
class TimeSeries;


class GenericDataStore: MarketDataStore {

	private:

		std::map<std::string, std::shared_ptr<HistoricData>> historic_data_store;
		std::map<std::string, std::shared_ptr<double>> latest_price_store;

	public:

		GenericDataStore();

		void addHistoricalData(std::string,TimeSeries&, TimeSeries&, TimeSeries&);
		void addHistoricalData(std::string, HistoricData);
		void addLatestPrice(std::string, double);
		void removeMarketData(std::string);
		
		const std::shared_ptr<HistoricData> historicalData(std::string) const;
		const std::shared_ptr<double> latestPrice(std::string) const;
		const TimeSeries& periodicData(std::string, TimeFrame) const;
		

		const std::map<std::string, std::shared_ptr<HistoricData>>& viewHistoricData() const;
		const std::map<std::string, std::shared_ptr<double>>& viewLatestPrices() const;
		std::vector<std::string> viewSymbols() const;

		void updateHistoricData(std::string, TimeSeries&, TimeSeries&, TimeSeries&);
		void updateLatestPrice(std::string, double);
		
};

#endif // !MARKET_DATA_STORE_HPP