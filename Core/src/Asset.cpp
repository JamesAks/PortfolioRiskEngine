#include "Asset.hpp"



Asset::Asset(std::string a_id, std::shared_ptr<HistoricData> hd, std::shared_ptr<LatestPrice> lp) :
	asset_id{a_id}, historical_data{hd}, latest_price{lp}, latest_valuation_date{latest_price->timestamp()} {
}


std::chrono::year_month_day Asset::latestValuationDate() const { return latest_valuation_date; }


std::shared_ptr<HistoricData> Asset::historicData() const { return historical_data; }


std::shared_ptr<LatestPrice> Asset::latestPrice() const { return latest_price; }


const std::string& Asset::symbol() const { return asset_id; }


double Asset::NPV(){ 

	calculateNPV();

	return net_present_value;
}

void Asset::update() { calculateNPV(); }