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
  