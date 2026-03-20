#include "TimeSeries.hpp"
#include <string>

class Asset{

    private:

        std::string name;
        int quantity;
        TimeSeries data;
        
    public:

        Asset(std::string&, int, TimeSeries);

        double currentPrice();
        TimeSeries historicData();
        std::string symbol();

};
