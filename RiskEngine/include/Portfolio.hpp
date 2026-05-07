#include "Asset.hpp"
#include <map>


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
        Position viewPosition(std::string);

        //Returns the positions held in the portfolio.
        std::vector<Position> viewPositions();

        // Returns the assets stored within the portfolio.
        std::vector<Asset> viewAssets();

        // Returns the label of all assets in the portfolio.
        std::vector<std::string> viewAssetLabels();

        // Returns the symbol/ID of the portfolio.
        std::string viewID();

        // Adds an asset to the portfolio.
        void addPosition(Position);

        // Removes asset from the portfolio.
        void removePosition(std::string);

        // Changes theID of the portfolio.
        void changeID(std::string);
        
        // Return the expected return of the portfolio.
        double expectedReturn();

        // Returns the weights of the assets in the portfolio.
        std::vector<double> weights();

        size_t size();
    };
