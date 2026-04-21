#include "../include/Asset.hpp"



Asset::Asset(std::string& n, size_t q, TimeSeries d) :name{ n }, quant{ q }, hist_data{ d }{};


TimeSeries Asset::historicData() { return hist_data; };


double Asset::currentPrice() {

    return hist_data.prices[hist_data.prices.size() - 1];
}


std::string Asset::symbol() { return name; };

double Asset::marketValue() {

    return currentPrice() * quant;
}

int Asset::quantity() { return quant; }