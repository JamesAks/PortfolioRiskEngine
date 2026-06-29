#include "Asset.hpp"
#include "Portfolio.hpp"
#include "Logger.hpp"



// ----- Private Members -----

void Portfolio::calculateWeights() {

    if (positions.size() == 0) { return; }
    double value = totalMarketValue();

    for (auto& p : positions) {

        position_weights[p.first] =  p.second->marketValue() / value;
    }
}

// ----- Public Memebers -----

Portfolio::Portfolio(std::string& n) : ID{ std::move(n) } {}

Portfolio::Portfolio(std::string n) : ID{ std::move(n) } {}


void Portfolio::addPosition(Position& p ) { 

    auto id = p.viewID();
    positions.emplace(id, std::make_unique<Position>(std::move(p)));
    Logger::logInfo("Added position: \"" + id + "\".");
    calculateWeights();
}


void Portfolio::removePosition(std::string& symbol) { positions.erase(symbol); }


void Portfolio::changeID(std::string new_ID) { ID = new_ID; }


double Portfolio::totalMarketValue() const {

    double sum = 0;

    for (auto& p : positions) {

        sum += p.second->marketValue();
    }

    return sum;
}


const std::unique_ptr<Position>& Portfolio::viewPosition(const std::string& symbol) const {

    if (positions.find(symbol) == positions.end()) {

        Logger::logError("Could not find position.");
        return nullptr;
    }

    return positions.find(symbol)->second;
}


const std::map<std::string, std::unique_ptr<Position>>& Portfolio::viewPositions() const { return positions; }


std::vector<std::shared_ptr<Asset>> Portfolio::viewAssets() const {

    std::vector<std::shared_ptr<Asset>> assets;

    for (auto& p : positions) {

        assets.push_back(p.second->viewAsset());
    }

    return assets;
}


std::vector<std::string> Portfolio::viewAssetLabels() const {

    std::vector<std::string> labels;

    for (auto& p : positions) {

        labels.push_back(p.first);
    }

    return labels;
}


const std::string& Portfolio::viewID() const { return ID; }

const std::map<std::string, double>& Portfolio::weights() const { return position_weights; }

size_t Portfolio::size() const { return positions.size(); }
