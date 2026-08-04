#ifndef HISTORIC_DATA_HPP
#define HISTORIC_DATA_HPP

#include "Observing.hpp"
#include "TimeSeries.hpp"

#include <vector>
#include <string>



// Classes that hold different forms of market data for a given underlying variable.

enum class TimeFrame { DAILY, WEEKLY, MONTHLY };

class HistoricData: public Observable {

	// Class that holds the historical periodic data of an underlying variable.

	private:

		TimeSeries daily;
		TimeSeries weekly;
		TimeSeries monthly;

	public:

		HistoricData() = default;
		HistoricData(const TimeSeries&, const TimeSeries&, const TimeSeries&);
		HistoricData(TimeSeries&&, TimeSeries&&, TimeSeries&&);

		HistoricData(size_t);

		void reserve(size_t);
		void addData(std::chrono::year_month_day, double, TimeFrame);
		void updateData(TimeSeries&, TimeFrame);

		const TimeSeries& dailyData() const;
		const TimeSeries& weeklyData() const;
		const TimeSeries& monthlyData() const;
		const TimeSeries& periodicPrices(TimeFrame) const;
		const std::vector<double>& periodicReturns(TimeFrame) const;
		size_t size(); 
};


class LatestPrice: public Observable {

	// Class that holds the latest price of an underlying variable.

	private:

		std::chrono::year_month_day date;
		double value;

	public:

		LatestPrice();
		LatestPrice(double, std::chrono::year_month_day);

		void updatePrice(double, std::chrono::year_month_day);

		double price() const;
		std::chrono::year_month_day timestamp() const;
};


#endif // !HISTORICA_DATA
