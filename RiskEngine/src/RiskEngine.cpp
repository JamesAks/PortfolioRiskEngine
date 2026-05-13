#include "../include/RiskEngine.hpp"
#include <iostream>


RiskEngine::RiskEngine(MarketDataManager& mdm): market_data_manager{ mdm }{}


AssetRiskReport RiskEngine::analyseAsset(const Position& pos, TimeFrame tf) const {

	
	Logger::logInfo("Analysing asset \"" + pos.asset->symbol() + "\".");
	RiskStatistics stats;
	std::string symbol = pos.asset->symbol();
	
	AssetRiskReport report{

		symbol,
		pos.asset->currentPrice(),
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
	
	RiskStatistics stats;
	return stats.periodicReturns(market_data_manager.periodicData(pos.asset->symbol(), tf).prices);
}
	

double RiskEngine::expectedReturn(const Position& pos, TimeFrame tf) const {
	
	RiskStatistics stats;
	return stats.mean(periodicReturns(pos, tf));
}


double RiskEngine::totalReturn(const Portfolio& port) const {

	double sum = 0;
	for (std::pair pos : port.viewPositions()) {

		sum += pos.second.quantity * pos.second.asset->currentPrice();
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

	RiskStatistics stats;
	std::vector<double> f_returns = periodicReturns(first, tf);
	std::vector<double> s_returns = periodicReturns(second, tf);

	return stats.covariance(f_returns, s_returns);
}


double RiskEngine::assetCorrelation(const Position& first, const Position& second, TimeFrame tf) const {

	RiskStatistics stats;
	std::vector<double> f_returns = periodicReturns(first,  tf);
	std::vector<double> s_returns = periodicReturns(second, tf);

	return stats.correlation(f_returns, s_returns);
}


std::vector<AssetRiskReport> RiskEngine::breakdown(const Portfolio& port, TimeFrame tf) const {

	std::vector<AssetRiskReport> breakdowns;
	for (std::pair pos : port.viewPositions()) {

		breakdowns.push_back(analyseAsset(pos.second, tf));
	}

	return breakdowns;
}


