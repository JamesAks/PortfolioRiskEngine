#include "../include/Portfolio.hpp"



Portfolio::Portfolio() :assets{ {} } {};


Portfolio::Portfolio(std::vector<Asset> as) : assets{ as } {};


std::vector<Asset> Portfolio::viewAssets(){

    return assets;
}


void Portfolio::add(Asset a) {

    assets.push_back(a);
}


void Portfolio::remove(std::string n) {

    for (Asset a : assets) {
        int i = 0;

        if (a.symbol() == n) {

            assets.erase(assets.begin() + i);
            break;
        }

        i++;
    }
}


size_t Portfolio::size() {

    return assets.size();
}
