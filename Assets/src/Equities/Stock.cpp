#include "Equities/Stock.hpp"
#include "MarketData.hpp"
#include "TimeSeries.hpp"



Stock::Stock(std::string st, std::shared_ptr<LatestPrice> lp, std::shared_ptr<HistoricData> hd) :
	Equity(st,hd,lp){}


void Stock::calculateNPV() { 
	
	net_present_value = latest_price->price(); 
}

