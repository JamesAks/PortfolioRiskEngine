#include "Equity.hpp"


// -----  Public Members -----
Equity::Equity(std::string a_id, std::shared_ptr<HistoricData> hd, std::shared_ptr<LatestPrice> lp): 
	Asset(a_id, hd, lp) {}

