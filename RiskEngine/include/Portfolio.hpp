#include "Asset.hpp"
#include <map>
#include <memory>


struct Position {

    size_t quantity = 0;
    std::shared_ptr<Asset> asset;
};

class Portfolio {

    // Portfolio class. Holds the list of assets within a portfolio
    private:

        std::string ID;
        std::map<std::string,Position> positions = {};
        
    public:

        Portfolio(std::string);

        // Returns a given position.
        Position viewPosition(std::string) const;

        //Returns the positions held in the portfolio.
        const std::map<std::string, Position>& viewPositions() const;

        // Returns the assets stored within the portfolio.
        std::vector<std::shared_ptr<Asset>> viewAssets() const;

        // Returns the label of all assets in the portfolio.
        std::vector<std::string> viewAssetLabels() const;

        // Returns the symbol/ID of the portfolio.
        std::string viewID() const;

        // Adds an asset to the portfolio.
        void addPosition(Position);

        // Removes asset from the portfolio.
        void removePosition(std::string);

        // Changes theID of the portfolio.
        void changeID(std::string);
        
        // Return the expected return of the portfolio.
        double expectedReturn() const;

        // Returns the weights of the assets in the portfolio.
        std::vector<double> weights() const;

        size_t size() const;
    };
