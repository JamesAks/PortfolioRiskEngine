#include "../include/RiskEngine.hpp"
#include <iostream>


RiskEngine::RiskEngine(MarketDataManager& mdm): market_data_manager{ mdm }{}


AssetRiskReport RiskEngine::analyseAsset(Position pos, TimeFrame tf) {

	
	RiskStatistics stats;
	std::string symbol = pos.asset->symbol();
	std::vector<double> data = market_data_manager.periodicData(symbol, tf).prices;
	
	AssetRiskReport report{

		symbol,
		pos.asset->currentPrice(),
		sqrt(stats.variance(data)),
		expectedReturn(pos, tf)
	};

	return report;
}


PortfolioRiskReport RiskEngine::analysePortfolio(Portfolio port, TimeFrame tf) {

	PortfolioRiskReport report{

		port.viewID(),
		totalReturn(port),
		expectedReturn(port,  tf),
		portfolioVolatility(port, tf),
		breakdown(port,  tf),
		computeCovarianceMatrix(port, tf)
	};

	return report;
}


std::vector<double> RiskEngine::periodicReturns(Position pos, TimeFrame tf) {
	
	RiskStatistics stats;
	return stats.periodicReturns(market_data_manager.periodicData(pos.asset->symbol(), tf).prices);
}
	

double RiskEngine::expectedReturn(Position pos, TimeFrame tf) {
	
	RiskStatistics stats;
	return stats.mean(periodicReturns(pos, tf));
}



double RiskEngine::totalReturn(Portfolio port) {

	double sum = 0;
	for (Position pos : port.viewPositions()) {

		sum += pos.quantity * pos.asset->currentPrice();
	}

	return sum;
}


double RiskEngine::expectedReturn(Portfolio port, TimeFrame tf) {

	auto ws = port.weights();
	double sum = 0;
	int i = 0;

	for (Position pos : port.viewPositions()) {

		sum += ws[i] * expectedReturn(pos, tf);
		i++;
	}

	return sum;
}


double RiskEngine::portfolioVolatility(Portfolio port, TimeFrame tf) {

	auto weights = port.weights();
	Eigen::MatrixXd cov_matrix = computeCovarianceMatrix(port, tf).data();
	Eigen::Map<Eigen::MatrixXd> weights_matrix(weights.data(), port.size(), 1);
	double variance = (weights_matrix.transpose() * cov_matrix * weights_matrix).value();
	return  sqrt(variance);
}


CovarianceMatrix RiskEngine::computeCovarianceMatrix(Portfolio port, TimeFrame tf){

	std::vector<Position> positions = port.viewPositions();
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


double RiskEngine::assetCovariance(Position first, Position second, TimeFrame tf) {

	RiskStatistics stats;
	std::vector<double> f_returns = periodicReturns(first, tf);
	std::vector<double> s_returns = periodicReturns(second, tf);

	return stats.covariance(f_returns, s_returns);
}


double RiskEngine::assetCorrelation(Position first, Position second, TimeFrame tf) {

	RiskStatistics stats;
	std::vector<double> f_returns = periodicReturns(first,  tf);
	std::vector<double> s_returns = periodicReturns(second, tf);

	return stats.correlation(f_returns, s_returns);
}


std::vector<AssetRiskReport> RiskEngine::breakdown(Portfolio port, TimeFrame tf) {

	std::vector<AssetRiskReport> breakdowns;
	for (Position pos : port.viewPositions()) {

		breakdowns.push_back(analyseAsset(pos, tf));
	}

	return breakdowns;
}


