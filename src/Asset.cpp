#include "../includes/Asset.hpp"

Asset::Asset(std::string& n, int q, TimeSeries d):name{n}, quantity{q}, data{d}{};

TimeSeries Asset::historicData(){return data;};

double Asset::currentPrice(){

    auto[dates,hist] = data.view();

    return hist[hist.size() - 1];
}

std::string Asset::symbol(){return name;};