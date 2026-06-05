#include "TimeSeries.hpp"
#include "Statistics.hpp"
#include "Logger.hpp"



// ----- Private Members -----

void TimeSeries::updateReturns(double price) {

	if (price_data.empty()) {
		return;
	}

	const double& latest = price_data[price_data.size() - 1];
	returns_data.push_back((price - latest) / latest);
}


void TimeSeries::calculateReturns() {

	if (returns_data.empty()) {
		return;
	}

	returns_data.reserve(price_data.size());

	for (size_t i = 1; i < price_data.size(); i++) {

		returns_data[i] = price_data[i] - price_data[i - 1] / price_data[i];
	}
}

// ----- Public Members -----

TimeSeries::TimeSeries(std::vector<std::chrono::year_month_day>& d, std::vector<double>& p) : date_data{ std::move(d) }, price_data{ std::move(p) } { calculateReturns(); };


TimeSeries::TimeSeries(size_t size) { reserve(size); }


const std::vector<std::chrono::year_month_day>& TimeSeries::dates() const { return date_data; }


const std::vector<double>& TimeSeries::prices() const { return price_data; }


void TimeSeries::addData(std::chrono::year_month_day date, double price) {

	date_data.push_back(date);
	updateReturns(price);
	price_data.push_back(price);

	
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


size_t TimeSeries::size() { return date_data.size(); }
