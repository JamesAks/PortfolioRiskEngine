#include "Asset.hpp"



class Portfolio {

    // Portfolio class. Holds the list of assets within a portfolio
    private:

        std::string name = "";
        std::vector<Asset> assets = {};
        
    public:

        Portfolio();
        Portfolio(std::string, std::vector<Asset>);

        // Returns the assets stored within the portfolio.
        std::vector<Asset> viewAssets();

        // Returns the symbol/ID of the portfolio.
        std::string viewName();

        // Adds an asset to the portfolio.
        void add(Asset);

        // Removes asset from the portfolio.
        void remove(std::string);

        // Changes the name of the portfolio.
        void changeName(std::string);
        
        // Return the expected return of the portfolio.
        double expectedReturn();

        // Returns the weights of the assets in the portfolio.
        std::vector<double> weights();

        size_t size();
    };
