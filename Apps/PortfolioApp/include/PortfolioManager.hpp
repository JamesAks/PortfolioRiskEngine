#ifndef PORTFOLIO_MANAGER_HPP
#define PORTFOLIO_MANAGER_HPP

#include <qstring.h>
#include <Memory>



class Asset;
class EfficientFrontier;
class MarketDataStore;
class Portfolio;
class PortfolioReport;
class Position;
enum class PositionType;
class Stock;

enum class TimeFrame;
        
class PortfolioManager {

    // Class that acts as interface between the Application UI(QT) and the business logic.

    private:

        std::map<std::string, std::shared_ptr<Portfolio>> portfolios;
        Portfolio* current_portfolio;
        MarketDataStore* market_data_store;

        std::shared_ptr<Stock> createStock(const std::string&) const;
        std::string avApiKey() const;

    public:

        PortfolioManager() = default;
        

        // Portfolio operations
        PortfolioReport analysePortfolio(TimeFrame) const;

        void createPortfolio(std::string);
        void removePortfolio(std::string);

        void addPortfolio(std::string);
        void removePortfolio(const QString&);

        Portfolio* currentPortfolio() const;
        void setCurrentPortfolio(QString);

        // Positon operations
        void addPosition(std::string, size_t, std::shared_ptr<Asset>, double, PositionType);
        void removePosition(std::string);

        double calculatePortfolioRisk(TimeFrame) const;
        double calculateSharpeRatio(TimeFrame) const;
        EfficientFrontier calculateEfficientFrontier(TimeFrame) const;

        size_t portfolioSize() const;
        size_t numbPortfolio() const;

        void registerMarketDataStore(MarketDataStore*);

        void updateAllMarketData();

        const std::map<std::string, std::shared_ptr<Portfolio>>& viewPortfolios() const;
        std::vector<std::string> viewPortfolioIDs() const;
		const Position& viewPosition(const QString&) const;

};

#endif // !PORTFOLIO_MANAGER_HPP