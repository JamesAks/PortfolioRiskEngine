#include "Asset.hpp"



class Portfolio {

    // Portfolio class. Holds the list of assets within a portfolio
    private:

        std::vector<Asset> assets;


    public:

        Portfolio();
        Portfolio(std::vector<Asset>);

        std::vector<Asset> viewAssets();
        void add(Asset);
        void remove(std::string);
        size_t size();

        void update();
    };
