#include "Asset.hpp"
#include "HistoricData.hpp"
#include "TimeSeries.hpp"


// ----- Private Class -----

// ----- Public Class -----

Asset::Asset(std::string s, std::shared_ptr<double> price, std::shared_ptr<HistoricData> hd):asset_symbol{ s }, latest_price{ price }, hist_data{hd}{}


double Asset::latestPrice() const { return *latest_price; }


const HistoricData& Asset::historicData() const { return *hist_data; }


const TimeSeries& Asset::periodicData(TimeFrame tf) const { return hist_data->periodicData(tf); }


const std::vector<double>& Asset::periodicReturns(TimeFrame tf) { return hist_data->periodicData(tf).returns(); }


std::string Asset::symbol() const { return asset_symbol; }



