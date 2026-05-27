#ifndef CSV_DATA_STORE_CPP
#define CSV_DATA_STORE_CPP


#include "MarketDataProvider.hpp"
#include "Core.hpp"
#include <iostream>
#include <fstream>   
#include <filesystem>


class CSVDataProvider : MarketDataProvider {

	private:

		void validateDirectory(std::string);
		std::string dir_path;

	public:

		
		CSVDataProvider(std::string);
		RequestResult periodicData(std::string, TimeFrame, size_t) const ;
		RequestResult latestPrice(std::string) const;

		void changeFile(std::string);
};

#endif // !CSV_DATA_STORE_CPP