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
        int quant;
        TimeSeries hist_data;

    public:

        Asset(std::string&, int, TimeSeries);

        // Returns the current/latest price of the asset.
        double currentPrice();

        // Returns the market value of the total asset including its quantity.
        double marketValue();

        // Returns quantity of the asset currently purchased.
        int quantity();

        // Returns the symbol/ID if the asset.
        std::string symbol();

        // Returns the historical data of the asset.
        TimeSeries historicData();

        
        
        
};
