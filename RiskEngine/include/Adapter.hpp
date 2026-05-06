#include <curl/curl.h>
#include <nlohmann/json.hpp>

enum class TimeFrame { DAILY, WEEKLY, MONTHLY };

struct TimeSeries {

	// Struct that holds the actual historical data of an asset
	std::vector<std::string> dates;
	std::vector<double> prices;
};

struct HistoricData {

	TimeSeries daily;
	TimeSeries weekly;
	TimeSeries monthly;

};



class Adapter {

	public:

		virtual std::string request(std::string) = 0;
		virtual TimeSeries historicalData(std::string, TimeFrame) = 0;
		virtual double latestPrice(std::string) = 0;
};


class AlphaVantageAdapter: public Adapter {

	private:

		std::string API_key;
		std::string base_url = "https://www.alphavantage.co/query?";
		std::string request(std::string);

	public:

		AlphaVantageAdapter(std::string);
		
		TimeSeries historicalData(std::string, TimeFrame);
		double latestPrice(std::string);

		nlohmann::json parse(std::string);
};


static size_t memoryWriteCallback( void* , size_t , size_t , void*);