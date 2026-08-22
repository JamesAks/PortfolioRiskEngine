#include "Asset.hpp"
#include "MarketData.hpp"


Asset::Asset(std::string a_id) :
	asset_id{ a_id }, net_present_value{0} {}


std::chrono::year_month_day Asset::latestValuationDate() const { return latest_valuation_date; }


AssetType Asset::assetType() const { return asset_type; }

const std::string& Asset::symbol() const { return asset_id; }


void Asset::update() {  }