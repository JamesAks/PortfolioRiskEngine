#ifndef MARKET_DATA_STORE_INTERFACE
#define MARKET_DATA_STORE_INTERFACE

#include <string>
#include <vector>
#include "Core.hpp"

class MarketDataStore {

private:

	std::map<std::string, HistoricData> historic_data_store;
	std::map<std::string, double> latest_price_store;

public:

	virtual void addHistoricalData(std::string, HistoricData) = 0;
	virtual void addLatestPrice(std::string, double) = 0;
	virtual void removeMarketData(std::string) = 0;

	virtual const HistoricData& historicalData(std::string) const = 0;
	virtual const TimeSeries& periodicData(std::string, TimeFrame) const = 0;
	virtual const double& currentPrice(std::string) const = 0;

	virtual const std::map<std::string, HistoricData>& viewHistoricData() const = 0;
	virtual const std::map<std::string, double>& viewLatestPrices() const = 0;
	virtual std::vector<std::string> viewSymbols() const = 0;

	virtual void updateHistoricData(std::string, HistoricData) = 0;
	virtual void updateLatestPrice(std::string, double) = 0;

};

#endif // !MARKET_DATA_STORE_INTERFACE