#ifndef TIME_SERIES_HPP
#define TIME_SERIES_HPP



struct TimeSeries {

	// Struct that holds the periodic historical data of an asset
	std::vector<std::string> dates;
	std::vector<double> prices;
};

#endif // !TIME_SERIES_HPP
