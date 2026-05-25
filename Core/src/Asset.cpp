#include "../include/Asset.hpp"



Asset::Asset(std::string s, std::shared_ptr<double> price, std::shared_ptr<HistoricData> hd):asset_symbol{ s }, latest_price{ price }, hist_data{hd}{}


double Asset::latestPrice() const { return *latest_price; }


const HistoricData& Asset::historicData() const { return *hist_data; }


const TimeSeries& Asset::periodicData(TimeFrame tf) const {

	switch (tf)
	{
	case TimeFrame::DAILY:

		return hist_data->daily;
		break;

	case TimeFrame::WEEKLY:

		return hist_data->weekly;
		break;

	case TimeFrame::MONTHLY:

		return hist_data->monthly;
		break;

	default:
		throw ("Invalid time frame given");
		break;
	}
}


std::string Asset::symbol() const { return asset_symbol; }



