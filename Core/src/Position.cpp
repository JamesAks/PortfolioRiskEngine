#include "../include/Position.hpp"



// ----- Publlic Members -----

Position::Position(std::string ID, size_t quant,  std::shared_ptr<Asset> as, double pba, PositionType pt):
	
	position_ID{ID},
	quantity{quant},
	asset{as},
	price_baught_at{pba},
	position_type{pt} {}


double Position::marketValue() const { return quantity * asset->latestPrice(); }


double Position::unrealizedGains() const {

	int quant = quantity;
	if (position_type == PositionType::SHORT) {
		quant *= -1;
	}

	return (asset->latestPrice() - price_baught_at) * quant;
}

double Position::initialInvestment() const { return quantity * price_baught_at; }


std::shared_ptr<Asset> Position::viewAsset() const { return asset; }


double Position::viewPriceBoughtAt() const { return price_baught_at; }


PositionType Position::viewPositionType() const { return position_type; }


size_t Position::viewQuantity() const { return quantity; }

std::string Position::viewID() const { return position_ID; }


