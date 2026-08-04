#include "MarketData.hpp"



// ----- HistoricData -----

// --- Public Members ---

HistoricData::HistoricData(const TimeSeries& d, const TimeSeries& w,const TimeSeries& m) : daily(d), weekly(w), monthly(m) {}


HistoricData::HistoricData(TimeSeries&& d, TimeSeries&& w,TimeSeries&& m) : daily(std::move(d)), weekly(std::move(w)), monthly(std::move(m)) {}


HistoricData::HistoricData(size_t size) { daily.reserve(size); weekly.reserve(size); monthly.reserve(size); }


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

	notifyObservers();
}


void HistoricData::updateData(TimeSeries& data, TimeFrame tf) {

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

	notifyObservers();
}


const TimeSeries& HistoricData::dailyData() const { return daily; }

const TimeSeries& HistoricData::weeklyData() const { return weekly; }

const TimeSeries& HistoricData::monthlyData() const { return monthly; }

const TimeSeries& HistoricData::periodicPrices(TimeFrame tf) const {

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


const std::vector<double>& HistoricData::periodicReturns(TimeFrame tf) const {

	switch (tf)
	{
	case TimeFrame::DAILY:

		return daily.returns();

	case TimeFrame::WEEKLY:

		return weekly.returns();

	case TimeFrame::MONTHLY:

		return monthly.returns();

	default:
		throw ("Invalid TimeFrame.");
		break;
	}
}


size_t HistoricData::size() { return daily.size() + weekly.size() + monthly.size(); }



// ----- LatestPrice -----

// --- Public Members ---

LatestPrice::LatestPrice() : value{ 0 }, date{ std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(1)) } {}

LatestPrice::LatestPrice(double v, std::chrono::year_month_day d) : value{v}, date { d } {}

void LatestPrice::updatePrice(double price, std::chrono::year_month_day new_date) {

	value = price;
	date = new_date;

	notifyObservers();
}


double LatestPrice::price() const { return value; }


std::chrono::year_month_day LatestPrice::timestamp() const { return date; }


