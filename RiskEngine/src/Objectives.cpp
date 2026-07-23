#include "CovarianceMatrix.hpp"
#include "Objectives.hpp"

#include <numeric>



double MaximiseReturn::evaluate(const std::vector<double>& weights, const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix) const {

	auto e_weights = Eigen::Map<const Eigen::VectorXd>(weights.data(), weights.size());
	auto e_returns = Eigen::Map<const Eigen::VectorXd>(expected_returns.data(), expected_returns.size());
	return e_weights.dot(e_returns);

	//return std::inner_product(weights.begin(), weights.end(), expected_returns.begin(),0);
}

double MaximiseSharpeRatio::evaluate(const std::vector<double>& weights, const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix) const {

	auto e_weights = Eigen::Map<const Eigen::VectorXd>(weights.data(), weights.size());
	auto e_returns = Eigen::Map<const Eigen::VectorXd>(expected_returns.data(), expected_returns.size());

	return (e_weights.dot(e_returns)) / (e_weights.transpose() * cov_matrix.matrixData() * e_weights);
}


double MinimiseVolatility::evaluate(const std::vector<double>& weights, const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix) const {

	auto e_weights = Eigen::Map<const Eigen::VectorXd>(weights.data(), weights.size());
	return sqrt(e_weights.transpose() * cov_matrix.matrixData() * e_weights);
}