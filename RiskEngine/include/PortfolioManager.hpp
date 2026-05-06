#include "../include/RiskEngine.hpp"



enum class AssetType {STOCK};
             
class PortfolioManager {

    private:

        std::map<std::string, Portfolio> portfolios;
        AlphaVantageAdapter adp;
        MarketDataManager market_data_manager;
        RiskEngine risk_engine;


        void displayPortfolioReport(PortfolioRiskReport);
        void displayAssetReport(AssetRiskReport);
        std::string avApiKey();


    public:

        PortfolioManager();
        void analysePortfolio(std::string, TimeFrame);

        void createPortfolio(std::string);
        void removePortfolio(std::string);

        void addPosition(size_t, std::string, std::string);
        void removePosition(std::string, std::string);

        void updateHistoricMarketData();
};

 