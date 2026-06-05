#include "HistoricData.hpp"



// ----- Private Members -----

// ----- Public Members -----

HistoricData::HistoricData(TimeSeries& d, TimeSeries& w, TimeSeries& m) : daily(std::move(d)), weekly(std::move(w)), monthly(std::move(m)) {}


HistoricData::HistoricData(size_t size) { daily.reserve(size); weekly.reserve(size); monthly.reserve(size); }


const TimeSeries& HistoricData::dailyData() const { return daily; }

const TimeSeries& HistoricData::weeklyData() const { return weekly; }

const TimeSeries& HistoricData::monthlyData() const { return monthly; }

const TimeSeries& HistoricData::periodicData(TimeFrame tf) const {

	switch (tf)
	{
	case TimeFrame::DAILY:

		return daily;

	case TimeFrame::WEEKLY:

		return weekly;

	case TimeFrame::MONTHLY:

		return monthly;

	default:
		throw ("Invalid TimeFrame.");
		break;
	}
}


void HistoricData::reserve(size_t quantity){

	daily.reserve(quantity);
	weekly.reserve(quantity);
	monthly.reserve(quantity);
}


void HistoricData::addData(std::chrono::year_month_day date, double price, TimeFrame tf) {

	switch (tf)
	{
	case TimeFrame::DAILY:

		daily.addData(date, price);
		break;
	case TimeFrame::WEEKLY:
		weekly.addData(date, price);
		break;
	case TimeFrame::MONTHLY:
		monthly.addData(date, price);
		break;
	default:
		throw ("Invalid TimeFrame.");
		break;
	}
}


size_t HistoricData::size() { return daily.size() + weekly.size() + monthly.size(); }

void HistoricData::updateData(TimeSeries& data, TimeFrame tf){

	switch (tf)
	{
	case TimeFrame::DAILY:
		daily = std::move(data);
		break;
	case TimeFrame::WEEKLY:
		weekly = std::move(data);
		break;
	case TimeFrame::MONTHLY:
		monthly = std::move(data);
		break;
	default:
		throw ("Invalid TimeFrame.");
		break;
	}

}

