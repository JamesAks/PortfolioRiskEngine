#ifndef HISTORIC_DATA_HPP
#define HISTORIC_DATA_HPP

#include "TimeSeries.hpp"

#include <vector>
#include <string>



enum class TimeFrame { DAILY, WEEKLY, MONTHLY };

class HistoricData {

	private:

		TimeSeries daily;
		TimeSeries weekly;
		TimeSeries monthly;

	public:

		// A shared internal object for a collection of periodic market data
		HistoricData() = default;
		HistoricData(TimeSeries&, TimeSeries&, TimeSeries&);
		HistoricData(size_t);

		const TimeSeries& dailyData() const;
		const TimeSeries& weeklyData() const;
		const TimeSeries& monthlyData() const;
		const TimeSeries& periodicData(TimeFrame) const;

		void reserve(size_t);
		void addData(std::string, double, TimeFrame);
		void updateData(TimeSeries&, TimeFrame);

		size_t size();
		
	};

#endif // !HISTORICA_DATA
