#ifndef STATISTICS_HPP
#define STATISTICS_HPP

#include <vector>
#include <math.h>
#include <Eigen/Dense>


enum class ConfidenceLevel { NINETY_FIVE, NINETY_NINE, NINETY_NINE_FIVE, NINETY_NINE_NINE };

namespace RiskCalculations {

    // A namespace for calculations often needed across libraries.

    double mean(const std::vector<double>&);
    std::vector<double> periodicReturns(const std::vector<double>&);
    double standardDeviation(const std::vector<double>&);
    double variance(const std::vector<double>&);
    double covariance(const std::vector<double>&, const std::vector<double>&);
    double correlation(const std::vector<double>&, const std::vector<double>&);
    double zScores(ConfidenceLevel);

    double volatility(const std::vector<double>&, const Eigen::MatrixXd&);
    double sharpeRatio(const std::vector<double>&, const std::vector<double>&, double, const Eigen::MatrixXd&);
};

#endif // !STATISTICS_HPP
