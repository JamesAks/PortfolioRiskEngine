#include "../include/MarketDataManager.hpp"


// ---- Private Members -----


void MarketDataManager::checkHistoricData(std::string symbol) {

	if (historicData.find(symbol) == historicData.end()) {

		printf("Fetching historical data.");
		addHistoricData(symbol);
	}
}

void MarketDataManager::checkLatestPrices(std::string symbol) {

	if (latestPrices.find(symbol) == latestPrices.end()) {

		printf("Fetching latest markest data.");
		addLatestPrice(symbol);
	}
}



TimeSeries MarketDataManager::dailyData(std::string symbol) {
	
	checkHistoricData(symbol);
	return historicData.find(symbol)->second.daily;
}


TimeSeries MarketDataManager::weeklyData(std::string symbol) {

	checkHistoricData(symbol);
	return historicData.find(symbol)->second.weekly;
}


TimeSeries MarketDataManager::monthlyData(std::string symbol) {

	checkHistoricData(symbol);
	return historicData.find(symbol)->second.monthly;
}

// ---- Public Members -----

MarketDataManager::MarketDataManager(Adapter& adp) : adapter{ adp } {}


void MarketDataManager::addHistoricData(std::string symbol) {

	HistoricData hd;
	hd.daily = adapter.historicalData(symbol, TimeFrame::DAILY);
	hd.weekly = adapter.historicalData(symbol, TimeFrame::WEEKLY);
	hd.monthly = adapter.historicalData(symbol, TimeFrame::MONTHLY);

	historicData.emplace(symbol, hd);
}


void MarketDataManager::addLatestPrice(std::string symbol) {

	double lp = adapter.latestPrice(symbol);
	latestPrices.emplace(symbol, lp);
}


HistoricData MarketDataManager::historicalData(std::string symbol) {

	checkHistoricData(symbol);
	return historicData.find(symbol)->second;
}

TimeSeries MarketDataManager::periodicData(std::string symbol, TimeFrame tf) {

	switch (tf)
	{
	case TimeFrame::DAILY:

		return dailyData(symbol);
		break;

	case TimeFrame::WEEKLY:

		return weeklyData(symbol);
		break;

	case TimeFrame::MONTHLY:

		return monthlyData(symbol);
		break;
	}
}


double MarketDataManager::currentPrice(std::string symbol) {

	checkLatestPrices(symbol);
	return latestPrices.find(symbol)->second;
}


void MarketDataManager::updateHistoricData() {

	for (std::pair hd : historicData) {

		hd.second = historicalData(hd.first);
	}
}
	

void MarketDataManager::changeAdapter(Adapter& adp_pointer) { adapter = adp_pointer; }

