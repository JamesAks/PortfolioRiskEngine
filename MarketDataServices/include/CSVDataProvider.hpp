#ifndef CSV_DATA_STORE_CPP
#define CSV_DATA_STORE_CPP

#include "MarketDataProvider.hpp"



enum class TimeFrame;

class CSVDataProvider : public MarketDataProvider {

	private:

		void validateDirectory(std::string);
		std::string dir_path;

	public:

		CSVDataProvider();
		CSVDataProvider(std::string);

		RequestResult periodicData(std::string, TimeFrame, size_t) const override;
		RequestResult latestPrice(std::string) const override;

		void changeFile(std::string);
};

#endif // !CSV_DATA_STORE_CPP