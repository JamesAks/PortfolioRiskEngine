#ifndef TIME_SERIES_HPP
#define TIME_SERIES_HPP

#include <chrono>
#include <vector>
#include <string>



class TimeSeries {

	private:

		// Struct that holds the periodic historical data of an asset
		std::vector<std::chrono::year_month_day> date_data;
		std::vector<double> price_data;
		std::vector<double> returns_data; 

		void updateReturns(double);
		void calculateReturns();

		//void validateDate();

	public:

		TimeSeries() = default;
		TimeSeries(std::vector<std::chrono::year_month_day>&, std::vector<double>&);
		TimeSeries(size_t);

		const std::vector<std::chrono::year_month_day>& dates() const;
		const std::vector<double>& prices() const;
		const std::vector<double>& returns() const;

		void addData(std::chrono::year_month_day, double);
		void reserve(size_t);
		

		size_t size();
};
   
#endif // !TIME_SERIES_HPP
