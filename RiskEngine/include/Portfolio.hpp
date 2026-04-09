#include "Asset.hpp"



class Portfolio {

    // Portfolio class. Holds the list of assets within a portfolio
    private:

        std::string name = "";
        std::vector<Asset> assets = {};
        
    public:

        Portfolio();
        Portfolio(std::string, std::vector<Asset>);

        std::vector<Asset> viewAssets();
        std::string viewName();
        void add(Asset);
        void remove(std::string);
        void changeName(std::string);
        

        double expectedReturn();
        std::vector<double> weights();
        size_t size();


    };
