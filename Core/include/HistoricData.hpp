#ifndef HISTORIC_DATA_HPP
#define HISTORIC_DATA_HPP

#include "TimeSeries.hpp"

enum class TimeFrame { DAILY, WEEKLY, MONTHLY };

struct HistoricData {

	// A shared internal object for a collection of periodic market data
	TimeSeries daily;
	TimeSeries weekly;
	TimeSeries monthly;
};

#endif // !HISTORICA_DATA
