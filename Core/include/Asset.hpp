#ifndef ASSET_HPP
#define ASSET_HPP

#include <chrono>
#include <string>
#include <vector>
#include <memory>

#include "Observing.hpp"



class HistoricData;
class LatestPrice;

enum class AssetType { NA, STOCK ,NUMBER_OF_TYPES};

class Asset : public Observable, public Observer {

    // Base class for an asset. 
   
    protected:

        std::string asset_id;
        double net_present_value;
        AssetType asset_type = AssetType::NA;
         
        std::shared_ptr<HistoricData> historical_data;
        std::shared_ptr<LatestPrice> latest_price;
        std::chrono::year_month_day latest_valuation_date;

        // Derived classes need to implement this method to calculate net present value.
        virtual void calculateNPV() {};

    public:
        
    
        Asset(std::string, std::shared_ptr<HistoricData>, std::shared_ptr<LatestPrice>);
         
        std::chrono::year_month_day latestValuationDate() const;
        std::shared_ptr<HistoricData> historicData() const;
        std::shared_ptr<LatestPrice> latestPrice() const;

        AssetType assetType() const;
        const std::string& symbol() const;
        double NPV();

        void update() override;

        virtual ~Asset() = default;
};

#endif // !ASSET_HPP