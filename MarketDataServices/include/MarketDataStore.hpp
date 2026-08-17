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

	virtual std::optional<HistoricData> getHistoricData(std::string) const = 0;
	virtual std::optional<LatestPrice> getLatestPrice(std::string) const = 0;
	virtual std::optional<MarketData> getMarketData(std::string) const = 0;

	virtual std::vector<std::optional<HistoricData>> getHistoricDatas(const std::vector<std::string>&) const = 0;
	virtual std::vector <std::optional<LatestPrice>> getLatestPrices(const std::vector<std::string>&) const = 0;
	virtual std::vector <std::optional<MarketData>> getMarketDatas(const std::vector<std::string>&) const = 0;

	virtual std::shared_ptr<const std::map<std::string, MarketData>> getMarketDataSnapshot() const = 0;

	virtual std::optional<TimeSeries> periodicData(std::string, TimeFrame) const = 0;

	virtual std::map<std::string, std::shared_ptr<HistoricData>> viewAllHistoricData() const = 0;
	virtual std::map<std::string, std::shared_ptr<LatestPrice>> viewAllLatestPrices() const = 0;

	virtual std::vector<std::string> viewSymbols() const = 0;

	virtual void update() = 0;

	virtual ~MarketDataStore() = default;
};

#endif // !MARKET_DATA_STORE_INTERFACE