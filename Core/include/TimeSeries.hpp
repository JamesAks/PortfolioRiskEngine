#ifndef TIME_SERIES_HPP
#define TIME_SERIES_HPP

#include <vector>
#include <string>



class TimeSeries {

	private:

		// Struct that holds the periodic historical data of an asset
		std::vector<std::string> date_data;
		std::vector<double> price_data;

	public:

		TimeSeries() = default;
		TimeSeries(std::vector<std::string>&, std::vector<double>&);
		TimeSeries(size_t);

		const std::vector<std::string>& dates() const;
		const std::vector<double>& prices() const;

		void addData(std::string, double);
		void reserve(size_t);

		size_t size();
};

#endif // !TIME_SERIES_HPP
