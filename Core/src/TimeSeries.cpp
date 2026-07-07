#include "TimeSeries.hpp"
#include "Statistics.hpp"
#include "Logger.hpp"



// ----- Private Members -----

void TimeSeries::updateReturns() {

	if (price_data.size() < 2) {
		return;
	}

	const double& latest = price_data[price_data.size() - 1];
	const double& prev = price_data[price_data.size() - 2];
	returns_data.push_back((latest - prev) / prev);
}


void TimeSeries::calculateReturns() {

	if (price_data.size() < 2) {
		return;
	}

	returns_data.reserve(price_data.size() - 1);

	for (size_t i = 1; i < price_data.size(); i++) {

		returns_data.push_back((price_data[i] - price_data[i - 1]) / price_data[i - 1]);
	}
}

// ----- Public Members -----

TimeSeries::TimeSeries(std::vector<std::chrono::year_month_day>& d, std::vector<double>& p){

	if (d.size() != p.size()) { throw ("Vector size mismatch."); }
	
	date_data = std::move(d);
	price_data = std::move(p);
	calculateReturns(); 
};


TimeSeries::TimeSeries(size_t size) { reserve(size); }


const std::vector<std::chrono::year_month_day>& TimeSeries::dates() const { return date_data; }


const std::vector<double>& TimeSeries::prices() const { return price_data; }


void TimeSeries::addData(std::chrono::year_month_day date, double price) {

	date_data.push_back(date);
	price_data.push_back(price);
	updateReturns();
}


const std::vector<double>& TimeSeries::returns() const { return returns_data; }


void TimeSeries::reserve(size_t size) {

	if (price_data.size() > size) {

		Logger::logError("TimeSeries has been allocated more space than requested already.");
		return;
	}

	date_data.reserve(size);
	price_data.reserve(size); 
}


size_t TimeSeries::size() const { return date_data.size(); }
