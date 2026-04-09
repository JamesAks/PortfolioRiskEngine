#include "vector"



class RiskStatistics {

    // A general statistic module that contains the mathematical methods (mean, variance, etc.) for anylsisng a sequence of data.

public:

    double mean(std::vector<double>);
    double standardDeviation(std::vector<double>);
    double variance(std::vector<double>);
    /*double covariance(std::vector<double>, std::vector<double>);*/
};
