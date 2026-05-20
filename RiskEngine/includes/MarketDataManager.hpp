#include <map>
#include <string>
#include "Adapter.hpp"

struct HistoricData {

	TimeSeries daily;
	TimeSeries weekly;
	TimeSeries monthly;

};

struct RequestStatus {

	RequestError resquest_error;
	std::string error_message;
};

class MarketDataManager {

	private:

		std::map<std::string, HistoricData> historicData;
		std::map<std::string, double> latestPrices;
		const Adapter& adapter;

		RequestError updateHistoricData(std::string);
		RequestError updateLatestPrice(std::string);

	public:

		MarketDataManager(Adapter&);

		RequestError addMarketData(std::string);

		const HistoricData& historicalData(std::string) const;
		const TimeSeries& periodicData(std::string, TimeFrame) const;
		const double& currentPrice(std::string) const;

		const std::map<std::string, HistoricData>& viewHistoricData() const;
		const std::map<std::string, double>& viewLatestPrices() const;
		std::vector<std::string> viewSymbols() const;
		void update();

};