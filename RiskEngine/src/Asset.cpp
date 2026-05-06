#include "../include/Asset.hpp"



Asset::Asset(std::string s, double price) :asset_symbol{ s }, current_price{ price }{};


double Asset::currentPrice() {

    return current_price;
}


void Asset::updatePrice(double price) {

    if (price < 0) {

        printf("Invalid - Price must be positive.");
    } 
    else {

        current_price = price;
    }
}


std::string Asset::symbol() { return asset_symbol; }



