#include "../include/RiskEngine.hpp"



enum class AssetType {STOCK};
             
class PortfolioManager {

    private:

        std::map<std::string, Portfolio> portfolios;
        AlphaVantageAdapter adp;
        MarketDataManager market_data_manager;
        const RiskEngine risk_engine;
        std::map<std::string,std::shared_ptr<Asset>> asset_store;

        void displayPortfolioReport(PortfolioRiskReport) const;
        void displayAssetReport(AssetRiskReport) const;
        std::string avApiKey() const;

    public:

        PortfolioManager();
        void analysePortfolio(std::string, TimeFrame) const;

        void createPortfolio(std::string);
        void removePortfolio(std::string);

        void addPosition(size_t, std::string, std::string);
        void removePosition(std::string, std::string);

        void updateMarketData();

};

 