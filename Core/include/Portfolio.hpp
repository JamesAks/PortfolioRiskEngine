#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include <map>
#include <memory>
#include <string>
#include <vector>


 
class Asset;
class Position;

class Portfolio {

    // Portfolio class. Holds the list of assets within a portfolio
    private:

        std::string ID;
        std::map<std::string,std::shared_ptr<Position>> positions;
        
    public:

        Portfolio(std::string&&);
        Portfolio(const std::string&);

        // Adds an asset to the portfolio.
        void addPosition(std::shared_ptr<Position>);
 
        // Removes asset from the portfolio.
        void removePosition(const std::string&);

        // Changes theID of the portfolio.
        void changeID(const std::string&);
        void changeID(std::string&&);

        // Return the expected return of the portfolio.
        double totalMarketValue() const;

        // Returns a given position.
        const Position& viewPosition(const std::string&) const;

        //Returns the positions held in the portfolio.
        const std::map<std::string, std::shared_ptr<Position>>& viewPositions() const;

        std::vector<std::string> viewPositionIDs() const;

        // Returns the assets stored within the portfolio.
        std::vector<std::shared_ptr<Asset>> viewAssets() const;

        // Returns the label of all assets in the portfolio.
        std::vector<std::string> viewAssetLabels() const;

        // Returns the symbol/ID of the portfolio.
        const std::string& viewID() const;

        // Returns the weights of the assets in the portfolio.
        std::map<std::string, double> weights() const;

        size_t size() const;
    };

#endif // !PORTOLIO_HPP