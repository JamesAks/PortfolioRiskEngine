#include "../includes/Asset.hpp"



Asset::Asset(std::string s,double price):asset_symbol{ s }, current_price{ price }{}


double Asset::currentPrice() const { return current_price; }


std::string Asset::symbol() const { return asset_symbol; }



