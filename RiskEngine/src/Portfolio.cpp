#include "../include/Portfolio.hpp"



Portfolio::Portfolio() :assets{ {} } {};


Portfolio::Portfolio(std::string n, std::vector<Asset> as) : name{ n }, assets { as } {};


std::vector<Asset> Portfolio::viewAssets() { return assets; }


std::vector<std::string> Portfolio::assetLabels() {

    std::vector<std::string> labels;

    for (Asset a : assets) {

        labels.push_back(a.symbol());
    }

    return labels;
}


std::string Portfolio::viewName() { return name; }


void Portfolio::add(Asset a) { assets.push_back(a); }


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


void Portfolio::changeName(std::string newName) { name = newName; }


double Portfolio::expectedReturn() {

    double sum = 0;

    for (Asset a : assets) {

        sum += a.marketValue();
    }

    return sum;
}

std::vector<double> Portfolio::weights() {

    std::vector<double> ws;
    double value = expectedReturn();

    for (Asset a : assets) {

        ws.push_back(a.marketValue() / value);
    }

    return ws;
}

size_t Portfolio::size() { return assets.size(); }
