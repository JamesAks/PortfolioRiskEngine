#include "../includes/Portfolio.hpp"

Portfolio::Portfolio():assets{{}}{};

Portfolio::Portfolio(vector<Asset> as): assets {as}{};

void Portfolio::add(Asset a){

    assets.push_back(a);
}

void Portfolio::remove(string n){

    for (Asset a: assets){
        int i = 0;

        if (a.symbol() == n){

            assets.erase(assets.begin() + i);
            break;
        }

        i++;
    }
}

size_t Portfolio::size(){

    return assets.size();
}
