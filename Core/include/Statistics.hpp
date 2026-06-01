#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>
#include <math.h>


// A general statistic module that contains the mathematical methods (mean, variance, etc.) for anylsisng a sequence of data.
class RiskStatistics {

    public:

        static double mean(std::vector<double>);
        static std::vector<double> periodicReturns(std::vector<double>);
        static double standardDeviation(std::vector<double>);
        static double variance(std::vector<double>);
        static double covariance(std::vector<double>, std::vector<double>);
        static double correlation(std::vector<double>, std::vector<double>);
    };

#endif // !STATISTICS_HPP
