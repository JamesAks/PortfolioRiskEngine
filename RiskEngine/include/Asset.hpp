#include <string>
#include <chrono>



struct TimeSeries {

    // Struct that holds the actual historical data of an asset
    std::vector<std::chrono::year_month_day> dates;
    std::vector<double> prices;
};


class Asset {

    // Very high-level, base class for an asset. Essentially an assets symbol(ticker name) and its historical data. Could be an option, bond etc.

    private:

        std::string name;
        int quantity;
        TimeSeries hist_data;

    public:

        Asset(std::string&, int, TimeSeries);

        double currentPrice();
        TimeSeries historicData();
        std::string symbol();
};
