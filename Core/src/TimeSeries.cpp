#include "TimeSeries.hpp"
#include "Statistics.hpp"
#include "Logger.hpp"



// ----- Private Members -----

// ----- Public Members -----

TimeSeries::TimeSeries(std::vector<std::string>& d, std::vector<double>& p) : date_data{ std::move(d) }, price_data{ std::move(p) } {};


TimeSeries::TimeSeries(size_t size) { reserve(size); }


const std::vector<std::string>& TimeSeries::dates() const { return date_data; }


const std::vector<double>& TimeSeries::prices() const { return price_data; }


void TimeSeries::addData(std::string date, double price) {

	date_data.push_back(date);
	price_data.push_back(price);
}


void TimeSeries::reserve(size_t size) {

	if (price_data.size() > size) {

		Logger::logError("TimeSeries has been allocated more space than requested already.");
		return;
	}

	date_data.reserve(size);
	price_data.reserve(size);
}


size_t TimeSeries::size() { return date_data.size(); }
