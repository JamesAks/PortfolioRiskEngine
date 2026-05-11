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

// ---- Public Members -----

MarketDataManager::MarketDataManager(Adapter& adp) : adapter{ adp } {}


RequestStatus MarketDataManager::addHistoricData(std::string symbol) {

	HistoricData hd;

	RequestResult daily = adapter.historicalData(symbol, TimeFrame::DAILY);
	if (daily.requestError != RequestError::NONE) {

		return { daily.requestError, daily.message };
	}

	RequestResult weekly = adapter.historicalData(symbol, TimeFrame::WEEKLY);
	if (weekly.requestError != RequestError::NONE) {

		return { weekly.requestError, daily.message };
	}

	RequestResult monthly = adapter.historicalData(symbol, TimeFrame::MONTHLY);
	if (monthly.requestError != RequestError::NONE) {

		return { monthly.requestError, daily.message };
	}

	hd.daily = daily.historicData.value();
	hd.weekly = weekly.historicData.value();
	hd.monthly = monthly.historicData.value();

	historicData.emplace(symbol, hd);

	return { RequestError::NONE, "" };
}



RequestStatus MarketDataManager::addLatestPrice(std::string symbol) {

	RequestResult lp = adapter.latestPrice(symbol);

	if (lp.requestError == RequestError::NONE) {

		latestPrices.emplace(symbol, lp.price);
		return  { RequestError::NONE, "" };
	
	}
	else {

		return { lp.requestError, lp.message };
	}
}


HistoricData MarketDataManager::historicalData(std::string symbol) const {

	auto result = historicData.find(symbol);
	return result->second;
}


TimeSeries MarketDataManager::periodicData(std::string symbol, TimeFrame tf) const {

	auto pd = historicData.find(symbol);
	switch (tf)
	{
	case TimeFrame::DAILY:

		return pd->second.daily;
		break;

	case TimeFrame::WEEKLY:

		return pd->second.weekly;
		break;

	case TimeFrame::MONTHLY:

		return pd->second.monthly;
		break;

	default:

		throw std::runtime_error("Invalid timeframe given.");
	}
}


const double& MarketDataManager::currentPrice(std::string symbol) const {

	return latestPrices.find(symbol)->second;
}


RequestStatus MarketDataManager::updateHistoricData(std::string symbol) {

	for (std::pair hd : historicData) {

		hd.second = historicalData(hd.first);
	}

	HistoricData hd = historicData.find(symbol)->second;

	RequestResult daily = adapter.historicalData(symbol, TimeFrame::DAILY);
	if (daily.requestError != RequestError::NONE) {

		return { daily.requestError, daily.message };
	}

	RequestResult weekly = adapter.historicalData(symbol, TimeFrame::WEEKLY);
	if (weekly.requestError != RequestError::NONE) {

		return { weekly.requestError, daily.message };
	}

	RequestResult monthly = adapter.historicalData(symbol, TimeFrame::MONTHLY);
	if (monthly.requestError != RequestError::NONE) {

		return { monthly.requestError, daily.message };
	}

	hd.daily = daily.historicData.value();
	hd.weekly = weekly.historicData.value();
	hd.monthly = monthly.historicData.value();

	historicData[symbol] = hd;

	return { RequestError::NONE, "" };
}


RequestStatus MarketDataManager::updateLatestData(std::string symbol) {

	RequestResult res = adapter.latestPrice(symbol);

	if (res.requestError != RequestError::NONE) {

		return { res.requestError, res.message };
	}

	latestPrices[symbol] = res.price;
	return { res.requestError, "" };
}


const std::map<std::string, HistoricData>& MarketDataManager::viewHistoricData() const { return historicData; }

const std::map<std::string, double>& MarketDataManager::viewLatestPrices() const { return latestPrices; }

std::vector<std::string> MarketDataManager::viewSymbols() const {
	
	std::vector<std::string> symbols;
	for (std::pair d : historicData) {

		symbols.push_back(d.first);
	}

	return symbols;
}
