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

		void checkHistoricData(std::string);
		void checkLatestPrices(std::string);
		RequestResult requestHistoricData(std::string);

	public:

		MarketDataManager(Adapter&);

		RequestStatus addHistoricData(std::string);
		RequestStatus addLatestPrice(std::string);

		HistoricData historicalData(std::string) const;
		TimeSeries periodicData(std::string, TimeFrame) const;
		const double& currentPrice(std::string) const;

		RequestStatus updateHistoricData(std::string);
		RequestStatus updateLatestData(std::string);

		const std::map<std::string, HistoricData>& viewHistoricData() const;
		const std::map<std::string, double>& viewLatestPrices() const;
		std::vector<std::string> viewSymbols() const;

};