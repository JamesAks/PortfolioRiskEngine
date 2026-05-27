#include "../include/CSVDataProvider.hpp"


// ----- Private Members -----

void CSVDataProvider::validateDirectory(std::string dir_path) {

	std::filesystem::path path(dir_path);

	if (!std::filesystem::is_directory(path)) {

		Logger::logError("\"" + dir_path + "\" is not a directory. Could not initialise CSV data provider.");
		throw "Invalid dirctory path.";
	}
}

// ----- Public Members -----

CSVDataProvider::CSVDataProvider(std::string fp) : dir_path{ fp } { validateDirectory(dir_path); }


RequestResult CSVDataProvider::periodicData(std::string asset_ID, TimeFrame tf, size_t quantity) const {

	std::ifstream market_data_file;
	std::string line;
	std::string file_name;
	int count = 0;

	switch (tf)
	{
	case TimeFrame::DAILY:
		file_name  = "Daily_Market_Data.csv";
		break;
	case TimeFrame::WEEKLY:
		file_name = asset_ID + "Weekly_Market_Data.csv";
		break;
	case TimeFrame::MONTHLY:
		file_name = asset_ID + " Monthly_Market_Data.csv";
		break;
	default:
		throw ("Invalid time frame.");
		break;
	}

	market_data_file.open(dir_path + "/" + asset_ID + "/" + file_name);

	if (market_data_file.fail()) {

		Logger::logError("Could not find appropriate file. Check spelling or directory.");
		return { std::nullopt, RequestError::INVALIDSYMBOL };
	}

	TimeSeries result;

	while (std::getline(market_data_file, line) && count < quantity) {

		auto seperator_pos = line.find(",");
		result.dates.push_back(line.substr(0, seperator_pos));
		result.prices.push_back(std::stod(line.substr(seperator_pos + 1, line.length())));
		count++;
	}
	market_data_file.close();

	if (count < quantity) {

		Logger::logWarning("The quantity requested was more than what was provided. Returning" + std::to_string(count) + " entrys.");
	}

	return { result, RequestError::NONE };
}


RequestResult CSVDataProvider::latestPrice(std::string asset_ID) const {

	std::ifstream lp_file;
	std::string line;
	lp_file.open(dir_path + "/" + asset_ID + "/Latest_Price.csv");

	if (lp_file.fail()) {

		Logger::logError("Could not find appropriate file. Check spelling or directory.");
		return { std::nullopt, RequestError::INVALIDSYMBOL };
	}

	getline(lp_file, line);
	lp_file.close();

	return { std::nullopt, RequestError::NONE, std::stod(line) };
}

void CSVDataProvider::changeFile(std::string file_path) { dir_path = file_path; }