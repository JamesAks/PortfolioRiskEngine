#ifndef ASSET_HPP
#define ASSET_HPP

#include <chrono>
#include <string>
#include <vector>
#include <memory>

#include "MarketData.hpp"
#include "Observing.hpp"
#include "TimeSeries.hpp"



class Asset : public Observable, public Observer {

    // Base class for an asset. 
    

    protected:

        std::string asset_id;
        std::shared_ptr<HistoricData> historical_data;
        std::shared_ptr<LatestPrice> latest_price;
        std::chrono::year_month_day latest_valuation_date;
        double net_present_value;

    public:
        
        Asset(std::string, std::shared_ptr<HistoricData>, std::shared_ptr<LatestPrice>);

        // Derived classes need to implement this method to calculate net present value.
        virtual void calculateNPV() = 0;

        const std::string& symbol() const;
        double NPV();
        std::chrono::year_month_day latestValuationDate() const ;
        std::shared_ptr<HistoricData> historicData() const ;
        std::shared_ptr<LatestPrice> latestPrice() const;

        void update() override;

};

#endif // !ASSET_HPP