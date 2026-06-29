#ifndef MARKET_DATA_STORE_INTERFACE
#define MARKET_DATA_STORE_INTERFACE

#include "TimeSeries.hpp"
#include "HistoricData.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>



class MarketDataStore {

public:

	virtual void addHistoricalData(std::string, TimeSeries&, TimeSeries&, TimeSeries&) = 0;
	virtual void addLatestPrice(std::string, LatestPrice) = 0;
	virtual void removeMarketData(std::string) = 0;

	virtual const std::shared_ptr<HistoricData> historicalData(std::string) const = 0;
	virtual const std::shared_ptr<LatestPrice> latestPrice(std::string) const = 0;
	virtual const TimeSeries& periodicData(std::string, TimeFrame) const = 0;

	virtual const std::map<std::string, std::shared_ptr<HistoricData>>& viewHistoricData() const = 0;
	virtual const std::map<std::string, std::shared_ptr<LatestPrice>>& viewLatestPrices() const = 0;
	virtual std::vector<std::string> viewSymbols() const = 0;

	virtual void updateHistoricData(std::string, TimeSeries&, TimeSeries&, TimeSeries&) = 0;
	virtual void updateLatestPrice(std::string, LatestPrice) = 0;
};

#endif // !MARKET_DATA_STORE_INTERFACE