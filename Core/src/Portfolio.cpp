#include "Asset.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "Logger.hpp"



// ----- Private Members -----


// ----- Public Memebers -----

Portfolio::Portfolio(std::string&& n): ID{std::move(n)}{}


Portfolio::Portfolio(const std::string& n) : ID{n} {}


void Portfolio::addPosition(std::shared_ptr<Position> position ) { 

    positions.emplace( position->viewID(), position);
    Logger::logInfo("Added position: \"" + position->viewID() + "\".");
} 


void Portfolio::removePosition(const std::string& symbol) { positions.erase(symbol); }


void Portfolio::changeID(const std::string& new_ID) { ID = new_ID; }


void Portfolio::changeID(std::string&& new_ID) { ID = std::move(new_ID); }


const Position& Portfolio::viewPosition(const std::string& symbol) const {

    return *positions.find(symbol)->second;
}


const std::map<std::string, std::shared_ptr<Position>>& Portfolio::viewPositions() const { return positions; }

std::vector<std::string> Portfolio::viewPositionIDs() const {

    std::vector<std::string> ids;

    for (auto& [name, position] : positions) {

        ids.push_back(name);
    }

    return ids;
}


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


size_t Portfolio::size() const { return positions.size(); }
