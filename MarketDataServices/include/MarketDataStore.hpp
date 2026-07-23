#ifndef MARKET_DATA_STORE_INTERFACE
#define MARKET_DATA_STORE_INTERFACE

#include "TimeSeries.hpp"
#include "MarketData.hpp"
#include "MarketDataProvider.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>



class MarketDataStore {

protected:

	virtual void updateHistoricData() = 0;
	virtual void updateLatestPrices() = 0;

public:

	virtual bool addMarketData(std::string) = 0;
	virtual bool removeMarketData(std::string) = 0;

	virtual const HistoricData& viewHistoricData(std::string) const = 0;
	virtual const LatestPrice& viewLatestPrice(std::string) const = 0;
	virtual const TimeSeries& periodicData(std::string, TimeFrame) const = 0;

	virtual const std::map<std::string, std::shared_ptr<HistoricData>>& viewAllHistoricData() const = 0;
	virtual const std::map<std::string, std::shared_ptr<LatestPrice>>& viewAllLatestPrices() const = 0;

	virtual std::vector<std::string> viewSymbols() const = 0;

	virtual ~MarketDataStore() = default;
};

#endif // !MARKET_DATA_STORE_INTERFACE