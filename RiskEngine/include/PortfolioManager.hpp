#include "RiskEngine.hpp"



enum class AssetType {STOCK};
             
class PortfolioManager {

    private:

        std::map<std::string, Portfolio> portfolios;
        AlphaVantageAdapter adp;
        MarketDataManager market_data_manager;
        const RiskEngine risk_engine;
        std::map<std::string,std::shared_ptr<Asset>> asset_store;

        void displayPortfolioReport(PortfolioRiskReport) const;
        void displayPositionReport(PositionRiskReport) const;
        std::string avApiKey() const;

    public:

        PortfolioManager();
        void analysePortfolio(std::string, TimeFrame) const;

        void createPortfolio(std::string);
        void removePortfolio(std::string);

        void addPosition(std::string, size_t, std::string, double, PositionType, std::string);
        void removePosition(std::string, std::string);

        size_t portfolioSize(std::string) const;
        size_t numbPortfolio() const;

        void updateMarketData();

};

 