#include "../include/Statistics.hpp"



// ----- Private Members -----

// ----- Publlic Members -----

double RiskStatistics::mean(std::vector<double> data) {

    double sum = 0;

    for (double d : data) {
        sum += d;
    }
    return sum / data.size();
}


std::vector<double> RiskStatistics::periodicReturns(std::vector<double> data) {

    std::vector<double> asset_returns;

    for (size_t i = 1; i < data.size(); i++) {

        asset_returns.push_back((data[i] - data[i - 1]) / data[i - 1]);
    }

    return asset_returns;
}


double RiskStatistics::variance(std::vector<double> data) {

    double m = mean(data);
    double temp = 0;

    for (double d : data) {

        temp += (d - m) * (d - m);
    }

    return temp / data.size();
}


double RiskStatistics::standardDeviation(std::vector<double> data) {

    return sqrt(variance(data));
}
 

double RiskStatistics::covariance(std::vector<double> first, std::vector<double> second) {

    double f_expected = mean(first);
    double s_expected = mean(second);

    double sum = 0;
    for (int i = 0; i < first.size(); i++) {

        sum += (first[i] - f_expected) * (second[i] - s_expected);
    }

    return sum / (first.size());
}

double RiskStatistics::correlation(std::vector<double> first, std::vector<double> second) {

    double covar = covariance(first, second);
    double f_std = standardDeviation(first);
    double s_std = standardDeviation(second);

    return covar / (f_std * s_std);
}