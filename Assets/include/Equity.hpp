#ifndef EQUITY_HPP
#define EQUITY_HPP

#include "Asset.hpp"

#include <chrono>
#include <string>
#include <vector>
#include <memory>

class HistoricData;
class LatestPrice;

class Equity: public Asset {

	// Base class for equities.

	protected:


	public:
		  
		Equity(std::string);

};
#endif // !EQUITY_HPP
