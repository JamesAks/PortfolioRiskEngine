#include "../includes/Portfolio.hpp"
#include <stdio.h>


int main(){
    
    auto a = year{2012}/2/3;
    vector<chrono::year_month_day> days {a, year{2012}/2/3, year{2012}/2/4};
    vector<double> prices {12.5, 16.4, 18.9};
    string name = "AMD";
    TimeSeries hist{days, prices};

    auto [dates,prices_h] = hist.view();

    for (auto x: prices_h)

        printf("%g \n", x);

    Asset asset{name, 10, hist};

    Portfolio portfolio;

    portfolio.add(asset);

    printf("Size of portfolio: %d \n", int(portfolio.size()));

    printf("Name of asset: %s \n", asset.symbol().c_str());

    printf("Current Asset price: %g \n", asset.currentPrice());
    
    portfolio.remove(name);

    printf("Size of portfolio: %d \n", int(portfolio.size()));

}