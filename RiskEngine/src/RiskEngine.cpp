#include "Asset.hpp"
#include "Logger.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"
#include "Statistics.hpp"
#include "TimeSeries.hpp"

#include <iostream>



// ----- Private Members -----

// ----- Public Members -----

RiskEngine::RiskEngine(){}


PositionRiskReport RiskEngine::analysePosition(const Position& pos, TimeFrame tf) const {

	Logger::logInfo("Analysing asset \"" + pos.viewID() + "\".");
	std::string symbol = pos.viewID();
	
	PositionRiskReport report{

		pos.viewID(),
		pos.viewQuantity(),
		pos.viewPriceBoughtAt(),
		pos.marketValue(),
		pos.initialInvestment(),
		pos.unrealizedGains(),

		pos.viewAsset()->symbol(),
		pos.viewAsset()->latestPrice(),
		RiskStatistics::standardDeviation(pos.viewAsset()->periodicReturns(tf)),
		expectedReturn(pos, tf)
	};

	return report;
}


PortfolioRiskReport RiskEngine::analysePortfolio(const Portfolio& port, TimeFrame tf) const {

	std::string portfolio_ID = port.viewID();
	Logger::logInfo("Analysing portfolio \"" + portfolio_ID + "\".");

	if (port.viewPositions().empty()) {

		Logger::logError("Can not anylse portfolio \"" + portfolio_ID + "\". No positions held in portfolio.");
		return {portfolio_ID, 0,0,0,{},};
	}

	PortfolioRiskReport report{

		portfolio_ID,
		totalReturn(port),
		expectedReturn(port,  tf),
		portfolioVolatility(port, tf),
		breakdown(port,  tf),
		computeCovarianceMatrix(port, tf)
	};

	return report;
}


std::vector<double> RiskEngine::portfolioPeriodicReturns(const Portfolio& port, TimeFrame tf, size_t quantity) const {

	std::vector<double> returns;
	returns.resize(quantity - 1, 0);
  	auto& weights = port.weights();

	for (const auto& pos : port.viewPositions()) {

		auto& pos_returns = pos.second->viewAsset()->periodicReturns(tf);
		auto weight = weights.find(pos.first);
		if (weight == weights.end()) { throw "Weight not found."; }

		for (int i = 0; i < quantity - 1; i++) {

			returns[i] += pos_returns[i] * weight->second;
		}
	}
	
	return returns;
}


double RiskEngine::expectedReturn(const Position& pos, TimeFrame tf) const { return RiskStatistics::mean(pos.viewAsset()->periodicReturns(tf)); }


double RiskEngine::totalReturn(const Portfolio& port) const {

	double sum = 0;
	for (auto& pos : port.viewPositions()) {

		sum += pos.second->marketValue();
	}

	return sum;
}


double RiskEngine::expectedReturn(const Portfolio& port, TimeFrame tf) const {

	auto& ws = port.weights();

	if (ws.size() == 0) {
		return NULL;
	}

	double sum = 0;
	int i = 0;

	for (auto& pos : port.viewPositions()) {

		sum += ws.find(pos.second->viewID())->second * expectedReturn(*pos.second, tf);
		i++;
	}

	return sum;
}


double RiskEngine::portfolioVolatility(const Portfolio& port, TimeFrame tf) const {

	std::vector<double> weights;
	for (auto ws : port.weights()) {

		weights.push_back(ws.second);
	}
	
	Eigen::MatrixXd cov_matrix = computeCovarianceMatrix(port, tf).matrixData();
	Eigen::Map<Eigen::MatrixXd> weights_matrix(weights.data(), port.size(), 1);
	double variance = (weights_matrix.transpose() * cov_matrix * weights_matrix).value();
	return  sqrt(variance);
}


CovarianceMatrix RiskEngine::computeCovarianceMatrix(const Portfolio& port, TimeFrame tf) const {

	int i = 0;
	CovarianceMatrix cov_matrix(port.viewAssetLabels());
	 
	for (auto& pos : port.viewPositions()) {

		int j = 0;

		for (auto& pos_2 : port.viewPositions()) {

			if (j < i) {

				j++;
				continue;
			}

			if (i == j) {

				cov_matrix(i, j) = assetCovariance(*pos.second, *pos_2.second, tf);
				j++;
				continue;
			}

			double covariance = assetCovariance(*pos.second, *pos_2.second, tf);
			
			cov_matrix(i, j) = covariance;
			cov_matrix(j, i) = covariance;

			j++;
		}

		i++;
	}

	return cov_matrix;
}


double RiskEngine::assetCovariance(const Position& first, const Position& second, TimeFrame tf) const {

	return RiskStatistics::covariance(first.viewAsset()->periodicReturns(tf), second.viewAsset()->periodicReturns(tf));
}


double RiskEngine::assetCorrelation(const Position& first, const Position& second, TimeFrame tf) const {

	return RiskStatistics::correlation(first.viewAsset()->periodicReturns(tf), second.viewAsset()->periodicReturns(tf));
} 


std::vector<PositionRiskReport> RiskEngine::breakdown(const Portfolio& port, TimeFrame tf) const {

	std::vector<PositionRiskReport> breakdowns;
	for (auto& pos : port.viewPositions()) {

		breakdowns.push_back(analysePosition(*pos.second, tf));
	}

	return breakdowns;
}


