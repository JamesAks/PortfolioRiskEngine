#include "MarketData.hpp"
#include "Equities/Stock.hpp"

// ----- Stock Class -----

// --- Private Members ---



// --- Public Members ---

Stock::Stock(std::string st):
	Equity(st) {
	
	asset_type = AssetType::STOCK;
}


