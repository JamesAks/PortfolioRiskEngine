#ifndef ASSET_HPP
#define ASSET_HPP

#include <chrono>
#include <string>
#include <vector>
#include <memory>

#include "HistoricData.hpp"
#include "TimeSeries.hpp"



class Asset {

    // Very high-level, base class for an asset. Essentially an assets symbol(ticker name) and its historical data. Could be an option, bond etc.
    

    protected:

        std::string asset_id;
        std::shared_ptr<HistoricData> historical_data;
        std::shared_ptr<LatestPrice> latest_price;
        std::chrono::year_month_day latest_valuation_date;
        double net_present_value;

    public:
        
        Asset(std::string, std::shared_ptr<HistoricData>, std::shared_ptr<LatestPrice>);

        // Calculates the net present value of the asset.
        virtual void calculateNPV() = 0;

        const std::string& symbol() const;
        double NPV();
        std::chrono::year_month_day latestValuationDate() const ;
        std::shared_ptr<HistoricData> historicData() const ;
        std::shared_ptr<LatestPrice> latestPrice() const;

};

#endif // !ASSET_HPP