#ifndef EQUITY_HPP
#define EQUITY_HPP

#include "Asset.hpp"

class Equity: public Asset {

	// Base class for equities.

	protected:


	public:
		  
		Equity(std::string, std::shared_ptr<HistoricData>, std::shared_ptr<LatestPrice>);

};
#endif // !EQUITY_HPP
