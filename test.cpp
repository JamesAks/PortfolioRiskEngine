#include "RiskEngine/include/RiskEngine.hpp"
#include <stdio.h>
#include <chrono>



int main(){
    
    auto a = std::chrono::year{2012}/2/3;
    std::vector<std::chrono::year_month_day> days {a, std::chrono::year{2012}/2/3, std::chrono::year{2012}/2/4};
    std::vector<double> prices {12.5, 16.4, 18.9};
    std::string name = "AMD";
    TimeSeries hist{days, prices};
    RiskStatistics rstats;
    RiskEngine riskEngine{rstats};

    for (auto x : hist.prices) {

        printf("%g \n", x);
    }

    Asset asset{name, 10, hist};

    Portfolio portfolio;

    portfolio.add(asset);

    printf("Size of portfolio: %d \n", int(portfolio.size()));

    printf("Current Asset price: %g \n", asset.currentPrice());
    
    portfolio.remove(name);

    printf("Size of portfolio: %d \n", int(portfolio.size()));

    riskEngine.analyseAsset(asset);
}