#include "Asset.hpp"
#include "Logger.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"
#include "Statistics.hpp"
#include "TimeSeries.hpp"

#include <iostream>



RiskEngine::RiskEngine(){}


PositionRiskReport RiskEngine::analysePosition(const Position& pos, TimeFrame tf) const {

	Logger::logInfo("Analysing asset \"" + pos.viewID() + "\".");
	RiskStatistics stats;
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
		stats.standardDeviation(periodicReturns(pos, tf)),
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


std::vector<double> RiskEngine::periodicReturns(const Position& pos, TimeFrame tf) const {

	return RiskStatistics::periodicReturns(pos.viewAsset()->periodicData(tf).prices());
}

	

double RiskEngine::expectedReturn(const Position& pos, TimeFrame tf) const { return RiskStatistics::mean(periodicReturns(pos, tf)); }


double RiskEngine::totalReturn(const Portfolio& port) const {

	double sum = 0;
	for (std::pair pos : port.viewPositions()) {

		sum += pos.second.marketValue();
	}

	return sum;
}


double RiskEngine::expectedReturn(const Portfolio& port, TimeFrame tf) const {

	auto ws = port.weights();
	double sum = 0;
	int i = 0;

	for (std::pair pos : port.viewPositions()) {

		sum += ws[i] * expectedReturn(pos.second, tf);
		i++;
	}

	return sum;
}


double RiskEngine::portfolioVolatility(const Portfolio& port, TimeFrame tf) const {

	auto weights = port.weights();
	Eigen::MatrixXd cov_matrix = computeCovarianceMatrix(port, tf).matrixData();
	Eigen::Map<Eigen::MatrixXd> weights_matrix(weights.data(), port.size(), 1);
	double variance = (weights_matrix.transpose() * cov_matrix * weights_matrix).value();
	return  sqrt(variance);
}


CovarianceMatrix RiskEngine::computeCovarianceMatrix(const Portfolio& port, TimeFrame tf) const {

	std::vector<Position> positions;

	for (std::pair pos : port.viewPositions()) {

		positions.push_back(pos.second);
	}

	size_t size = positions.size();
	CovarianceMatrix cov_matrix(port.viewAssetLabels());

	for (size_t i = 0; i < size; i++) {

		for (size_t j = i; j < size; j++) {

			double covariance = assetCovariance(positions[i], positions[j], tf);
			cov_matrix(i, j) = covariance;
			cov_matrix(j, i) = covariance;
		}
	}

	return cov_matrix;
}


double RiskEngine::assetCovariance(const Position& first, const Position& second, TimeFrame tf) const {

	std::vector<double> f_returns = periodicReturns(first, tf);
	std::vector<double> s_returns = periodicReturns(second, tf);

	return RiskStatistics::covariance(f_returns, s_returns);
}


double RiskEngine::assetCorrelation(const Position& first, const Position& second, TimeFrame tf) const {

	std::vector<double> f_returns = periodicReturns(first,  tf);
	std::vector<double> s_returns = periodicReturns(second, tf);

	return RiskStatistics::correlation(f_returns, s_returns);
}


std::vector<PositionRiskReport> RiskEngine::breakdown(const Portfolio& port, TimeFrame tf) const {

	std::vector<PositionRiskReport> breakdowns;
	for (std::pair pos : port.viewPositions()) {

		breakdowns.push_back(analysePosition(pos.second, tf));
	}

	return breakdowns;
}


