#ifndef MARKET_DATA_PROVIDER_INTERFACE_HPP
#define MARKET_DATA_PROVIDER_INTERFACE_HPP



struct RequestResult {

	std::optional<TimeSeries> historicData;
	RequestError requestError;
	double price;
};


class MarketDataProvider {

	public:

		virtual RequestResult periodicData(std::string, TimeFrame) const = 0;
		virtual RequestResult latestPrice(std::string) const = 0;
};

#endif // !MARKET_DATA_PROVIDER_INTERFACE_HPP