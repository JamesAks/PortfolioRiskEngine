#include "../include/Statistic.hpp"
#include <math.h>



double RiskStatistics::mean(std::vector<double> data) {

    double sum = 0;

    for (double d : data) {
        sum += d;
    }
    return sum / data.size();
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