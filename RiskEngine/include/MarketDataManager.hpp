#include <map>
#include <string>
#include "Adapter.hpp"


 
class MarketDataManager {

	private:

		std::map<std::string, HistoricData> historicData;
		std::map<std::string, double> latestPrices;
		Adapter& adapter;

		void checkHistoricData(std::string);
		void checkLatestPrices(std::string);

		TimeSeries dailyData(std::string);
		TimeSeries weeklyData(std::string);
		TimeSeries monthlyData(std::string);

	public:

		MarketDataManager(Adapter&);

		void addHistoricData(std::string);
		void addLatestPrice(std::string);

		HistoricData historicalData(std::string);
		TimeSeries periodicData(std::string, TimeFrame);
		double currentPrice(std::string);

		void updateHistoricData();
		void changeAdapter(Adapter&);

};