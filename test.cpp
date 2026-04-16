#include "RiskEngine/include/RiskEngine.hpp"
#include <stdio.h>
#include <chrono>
#include <Eigen/Dense>
#include <iostream>



int main() {

    auto a = std::chrono::year{ 2012 } / 2 / 3;
    std::vector<std::chrono::year_month_day> days{ a, std::chrono::year{2012} / 2 / 3, std::chrono::year{2012} / 2 / 4 };
    std::vector<double> prices{ 12.5, 16.4, 18.9 };
    std::string name1 = "AMD";
    std::string name2 = "APPL";
    std::string name3 = "TSL";
    std::string name4 = "SSG";
    TimeSeries hist{ days, prices };
    RiskStatistics r_stats;
    RiskEngine risk_engine{ r_stats };

    for (auto x : hist.prices) {

        printf("%g \n", x);
    }

    Asset asset1{ name1, 1, hist };
    Asset asset2{ name2, 2, hist };
    Asset asset3{ name3, 3, hist };
    Asset asset4{ name4, 4, hist };

    Portfolio portfolio;

    portfolio.add(asset1);
    portfolio.add(asset2);
    portfolio.add(asset3);
    portfolio.add(asset4);

    auto report = risk_engine.analysePortfolio(portfolio);
    displayPortfolioReport(report);

    printf("Size of portfolio: %d \n", int(portfolio.size()));

    printf("Current Asset price: %g \n", asset1.currentPrice());

    portfolio.remove(name1);

    printf("Size of portfolio: %d \n", int(portfolio.size()));


}