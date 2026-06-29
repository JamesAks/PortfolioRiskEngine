#include "Stock.hpp"
#include "HistoricData.hpp"
#include "TimeSeries.hpp"



Stock::Stock(std::string st, std::shared_ptr<LatestPrice> lp, std::shared_ptr<HistoricData> hd) :
	Asset(st,hd,lp){ }


void Stock::calculateNPV() { 
	
	net_present_value = latest_price->value; 
}

