#include "Asset.hpp"

Asset::Asset( std::string ai, std::shared_ptr<HistoricData> hd, std::shared_ptr<LatestPrice> lp):
	asset_id{ai}, historical_data{hd}, latest_price{ lp } {
}


const std::string& Asset::symbol() const { return asset_id; }

double Asset::NPV(){ 

	if (net_present_value == NULL) {
		calculateNPV();
	} 

	return net_present_value;
}


std::chrono::year_month_day Asset::latestValuationDate() const { return latest_valuation_date; }


std::shared_ptr<HistoricData> Asset::historicData() const { return historical_data; }


std::shared_ptr<LatestPrice> Asset::latestPrice() const { return latest_price; }