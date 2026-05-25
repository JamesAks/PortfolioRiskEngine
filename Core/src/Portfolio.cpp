#include "../include/Portfolio.hpp"



Portfolio::Portfolio(std::string n) : ID{ n } {}

Position Portfolio::viewPosition(std::string symbol) const {

    return positions.find(symbol)->second;
}


const std::map<std::string,Position>& Portfolio::viewPositions() const { return positions; }


std::vector<std::shared_ptr<Asset>> Portfolio::viewAssets() const {
    
    std::vector<std::shared_ptr<Asset>> assets;

    for (std::pair p : positions) {

        assets.push_back(p.second.viewAsset());
    }

    return assets;
}


std::vector<std::string> Portfolio::viewAssetLabels() const {

    std::vector<std::string> labels;

    for (std::pair p : positions) {

        labels.push_back(p.first);
    }

    return labels;
}


std::string Portfolio::viewID() const { return ID; }


void Portfolio::addPosition(Position p ) { positions.emplace(p.viewID(), p); }


void Portfolio::removePosition(std::string symbol) { positions.erase(symbol); }


void Portfolio::changeID(std::string new_ID) { ID = new_ID; }


double Portfolio::totalMarketValue() const {

    double sum = 0;

    for (std::pair p : positions) {

        sum += p.second.marketValue();
    }

    return sum;
}


std::vector<double> Portfolio::weights() const {

    std::vector<double> ws;
    double value = totalMarketValue();

    for (std::pair p : positions) {

        ws.push_back((p.second.marketValue()/ value));
    }

    return ws;
}

size_t Portfolio::size() const { return positions.size(); }
