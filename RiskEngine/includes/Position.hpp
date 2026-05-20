#include "Asset.hpp"
#include <map>
#include <memory>



enum class PositionType { LONG, SHORT, NA };

class Position {

	private:

        std::string position_ID;
        size_t quantity = 0;
        std::shared_ptr<Asset> asset;
        double price_baught_at = 0;
        PositionType position_type = PositionType::NA;

    public:

        Position(std::string, size_t, std::shared_ptr<Asset>, double, PositionType);
        double marketValue() const;
        double unrealizedGains() const;
        double initialInvestment() const;
        

        std::shared_ptr<Asset> viewAsset() const;
        double viewPriceBoughtAt() const;
        PositionType viewPositionType() const;
        size_t viewQuantity() const;
        std::string viewID() const;
};

