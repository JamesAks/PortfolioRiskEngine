#include <curl/curl.h>
#include <nlohmann/json.hpp>



class Adapter {

	public:

		virtual std::string request(std::string) = 0;

		virtual std::pair<std::vector<std::string>, std::vector<double>>  historicalDaily(std::string) = 0;
		virtual std::pair<std::vector<std::string>, std::vector<double>> historicalMonthly(std::string) = 0;
		virtual std::string formURL(std::string) = 0;

		virtual ~Adapter() = default;
};


class AlphaVantageAdapter: public Adapter {

	private:

		std::string API_key;
		std::string base_url = "https://www.alphavantage.co/query?";

	public:

		AlphaVantageAdapter(std::string);
		
		std::string request(std::string);

		std::pair<std::vector<std::string>, std::vector<double>> historicalDaily(std::string);
		std::pair<std::vector<std::string>, std::vector<double>> historicalMonthly(std::string);
		std::string formURL(std::string);
		nlohmann::json parse(std::string);
};


static size_t memoryWriteCallback( void* , size_t , size_t , void*);