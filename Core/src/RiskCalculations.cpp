#include "RiskCalculations.hpp"



double RiskCalculations::mean(const std::vector<double>& data) {

    auto converted_data = Eigen::Map<const Eigen::VectorXd>(data.data(), data.size());
   
    return converted_data.mean();
}


std::vector<double> RiskCalculations::periodicReturns(const std::vector<double>& data) {

    std::vector<double> asset_returns;

    for (size_t i = 1; i < data.size(); i++) {

        asset_returns.push_back((data[i] - data[i - 1]) / data[i - 1]);
    }

    return asset_returns;
}


double RiskCalculations::variance(const std::vector<double>& data) {

    double m = mean(data);

    double temp = 0;

    for (double d : data) {

        temp += (d - m) * (d - m);
    }

    return temp / data.size();
}


double RiskCalculations::standardDeviation(const std::vector<double>& data) {

    return sqrt(variance(data));
}


double RiskCalculations::covariance(const std::vector<double>& first, const std::vector<double>& second) {

    double f_expected = mean(first);
    double s_expected = mean(second);

    double sum = 0;
    for (int i = 0; i < first.size(); i++) {

        sum += (first[i] - f_expected) * (second[i] - s_expected);
    }

    return sum / (first.size());
}


double RiskCalculations::correlation(const std::vector<double>& first, const std::vector<double>& second) {

    double covar = covariance(first, second);
    double f_std = standardDeviation(first);
    double s_std = standardDeviation(second);

    return covar / (f_std * s_std);
}


double RiskCalculations::zScores(ConfidenceLevel cl) {

    switch (cl)
    {
    case ConfidenceLevel::NINETY_FIVE:

        return 1.64485;
        break;

    case ConfidenceLevel::NINETY_NINE:

        return 2.32635;
        break;

    case ConfidenceLevel::NINETY_NINE_FIVE:

        return 2.57583;
        break;

    case ConfidenceLevel::NINET_NINE_NINE:

        return 3.09023;
        break;

    default:

        throw "Invalid Confidence Level.";
        break;
    }
}

double RiskCalculations::volatility(const std::vector<double>& weights, const Eigen::MatrixXd& cov_matrix) {

    auto weights_cpy = weights;
    Eigen::Map<Eigen::MatrixXd> e_weights(weights_cpy.data(), weights.size(), 1);

    return sqrt((e_weights.transpose() * cov_matrix * e_weights).value());
}

double RiskCalculations::sharpeRatio(const std::vector<double>& weights, const std::vector<double>& expected_returns, double risk_free_rate, const Eigen::MatrixXd& cov_matrix) {

    // Sharpe ratio: (expected returm - risk free rate) / portfolio volatility.

    auto e_weights = Eigen::Map<const Eigen::VectorXd>(weights.data(), weights.size());
    auto e_returns = Eigen::Map<const Eigen::VectorXd>(expected_returns.data(), expected_returns.size());

    return (e_weights.dot(e_returns) - risk_free_rate) / RiskCalculations::volatility(weights, cov_matrix);
}