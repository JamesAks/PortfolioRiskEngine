#ifndef PORTFOLIO_HPP
#define PORTFOLIO_HPP

#include "Position.hpp"

#include <map>
#include <memory>
#include <string>
#include <vector>


 

class Asset;

class Portfolio {

    // Portfolio class. Holds the list of assets within a portfolio
    private:

        std::string ID;
        std::map<std::string,std::unique_ptr<Position>> positions;
        std::map<std::string, double> position_weights;

        void calculateWeights();
        
    public:

        Portfolio(std::string&);
        Portfolio(std::string);


        // Adds an asset to the portfolio.
        void addPosition(Position&);

        // Removes asset from the portfolio.
        void removePosition(std::string&);

        // Changes theID of the portfolio.
        void changeID(std::string);
        
        // Return the expected return of the portfolio.
        double totalMarketValue() const;


        // Returns a given position.
        const std::unique_ptr<Position>& viewPosition(const std::string&) const;

        //Returns the positions held in the portfolio.
        const std::map<std::string, std::unique_ptr<Position>>& viewPositions() const;

        // Returns the assets stored within the portfolio.
        std::vector<std::shared_ptr<Asset>> viewAssets() const;

        // Returns the label of all assets in the portfolio.
        std::vector<std::string> viewAssetLabels() const;

        // Returns the symbol/ID of the portfolio.
        const std::string& viewID() const;

        // Returns the weights of the assets in the portfolio.
        const std::map<std::string, double>& weights() const;

        size_t size() const;
    };

#endif // !PORTOLIO_HPP