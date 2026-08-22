#ifndef POSITION_HPP
#define POSITION_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>



class Asset;
enum class PositionType { LONG, SHORT, NA };


class Position {

    // Class containing asset exposure.
	private:

        std::string id;
        size_t quantity = 0;
        std::shared_ptr<Asset> asset;
        double price_baught_at = 0;
        PositionType position_type = PositionType::NA;

    public:

        Position(std::string, size_t, std::shared_ptr<Asset>, double, PositionType);

        // Returns how much was originally invested into asset
        double initialInvestment() const;

        std::shared_ptr<Asset> viewAsset() const;
        double viewPriceBoughtAt() const;
        PositionType viewPositionType() const;
        size_t viewQuantity() const;
        const std::string viewID() const;
};

#endif // !POSITION_HPP