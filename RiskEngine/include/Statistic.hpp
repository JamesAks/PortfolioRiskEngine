#include "vector"


// A general statistic module that contains the mathematical methods (mean, variance, etc.) for anylsisng a sequence of data.
class RiskStatistics {

    public:

        double mean(std::vector<double>) const;
        std::vector<double> periodicReturns(std::vector<double>) const;
        double standardDeviation(std::vector<double>) const;
        double variance(std::vector<double>) const;
        double covariance(std::vector<double>, std::vector<double>) const;
        double correlation(std::vector<double>, std::vector<double>) const;
    };
