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
        std::chrono::year_month_day latest_valuation_date;

    public:
        
    
        Asset(std::string);
         
        std::chrono::year_month_day latestValuationDate() const;

        AssetType assetType() const;
        const std::string& symbol() const;

        void update() override;

        virtual ~Asset() = default;
};

#endif // !ASSET_HPP