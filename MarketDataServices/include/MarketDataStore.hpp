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


public:

	virtual bool addMarketData(std::string) = 0;
	virtual bool removeMarketData(std::string) = 0;

	virtual std::shared_ptr<const std::map<std::string, MarketData>> getMarketDataSnapshot() const = 0;

	virtual void update() = 0;

	virtual ~MarketDataStore() = default;
};

#endif // !MARKET_DATA_STORE_INTERFACE