#include "CovarianceMatrix.hpp"
#include "Logger.hpp"
#include "Objectives.hpp"
#include "RiskCalculations.hpp"



#include <numeric>

// ----- MaximiseReturn -----

// --- Private Members ---

// --- Public Members ---

double MaximiseReturn::evaluate(const std::vector<double>& weights, const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix){

	auto e_weights = Eigen::Map<const Eigen::VectorXd>(weights.data(), weights.size());
	auto e_returns = Eigen::Map<const Eigen::VectorXd>(expected_returns.data(), expected_returns.size());
	return e_weights.dot(e_returns);

	//return std::inner_product(weights.begin(), weights.end(), expected_returns.begin(),0);
}




// ----- MaximiseSharpeRatio -----

// --- Private Members ---

// --- Public Members ---

double MaximiseSharpeRatio::evaluate(const std::vector<double>& weights, const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix) {
	
	return RiskCalculations::sharpeRatio(weights, expected_returns, risk_free_rate, cov_matrix.matrixData());
}

void MaximiseSharpeRatio::changeRiskFreeRate(double new_rate) {

	if (risk_free_rate < 0) {
		Logger::logError("Unable to change risk free rate. New rate must be greater than 0.");
	}

	risk_free_rate = new_rate;
}



// ----- MinimiseVolatility -----

// --- Private Members ---

// --- Public Members ---

double MinimiseVolatility::evaluate(const std::vector<double>& weights, const std::vector<double>& expected_returns, const CovarianceMatrix& cov_matrix) {

	return RiskCalculations::volatility(weights, cov_matrix.matrixData());
}
