#include "Asset.hpp"
#include "Logger.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"
#include "Statistics.hpp"
#include "TimeSeries.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

constexpr double PI = 3.14159265358979323846;

PositionRiskReport RiskEngine::analysePosition(const Position& pos, TimeFrame tf)  {

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
		pos.viewAsset()->NPV(),
		expectedReturn(pos, tf),
	};

	return report;
}


PortfolioRiskReport RiskEngine::analysePortfolio(const Portfolio& port, TimeFrame tf)  {

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
		historicalVaR(port,tf, 100, 0.95),
		historicalShortfall(port,tf, 100, 0.95),
		parametricVaR(port,tf, 100, ConfidenceLevel::NINETY_FIVE),
		parametricShortfall(port,tf, 100, ConfidenceLevel::NINETY_FIVE),
		portfolioSharpeRatio(port, tf, 100, 0.04),
		breakdown(port,  tf),
		computeCovarianceMatrix(port, tf)
	};

	return report;
}


std::vector<double> RiskEngine::portfolioPeriodicReturns(const Portfolio& port, TimeFrame tf, size_t quantity)  {

	std::vector<double> returns;
	returns.resize(quantity - 1, 0);
  	auto& weights = port.weights();

	for (const auto& pos : port.viewPositions()) {

		auto& pos_returns = pos.second->viewAsset()->historicData()->periodicReturns(tf);
		auto weight = weights.find(pos.first);
		if (weight == weights.end()) { throw "Weight not found."; }

		for (int i = 0; i < quantity - 1; i++) {

			returns[i] += pos_returns[i] * weight->second;
		}
	}
	
	return returns;
}


double RiskEngine::expectedReturn(const Position& pos, TimeFrame tf)  { return RiskStatistics::mean(pos.viewAsset()->historicData()->periodicReturns(tf)); }


double RiskEngine::totalReturn(const Portfolio& port)  {

	double sum = 0;
	for (auto& pos : port.viewPositions()) {

		sum += pos.second->marketValue();
	}

	return sum;
}


double RiskEngine::expectedReturn(const Portfolio& port, TimeFrame tf)  {

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


double RiskEngine::portfolioVolatility(const Portfolio& port, TimeFrame tf) {

	std::vector<double> weights;
	for (auto& ws : port.weights()) {

		weights.push_back(ws.second);
	}
	
	Eigen::MatrixXd cov_matrix = computeCovarianceMatrix(port, tf).matrixData();
	Eigen::Map<Eigen::MatrixXd> weights_matrix(weights.data(), port.size(), 1);
	double variance = (weights_matrix.transpose() * cov_matrix * weights_matrix).value();
	return sqrt(variance);
}


CovarianceMatrix RiskEngine::computeCovarianceMatrix(const Portfolio& port, TimeFrame tf)  {

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


double RiskEngine::historicalVaR(const Portfolio& port, TimeFrame tf, size_t quantity, double confidence)  {

	if (confidence > 1 || confidence < 0 ) {

		Logger::logError("Confidence should be between 1 and 0.");
		return 0;
	}

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	std::sort(returns.begin(), returns.end(), std::greater<double>());

	//for (auto ret : returns) {

	//	Logger::logDebug(std::to_string(ret));
	//}
	
	return - (totalReturn(port) * returns[size_t(floor(returns.size() * confidence))]);
}


double RiskEngine::historicalShortfall(const Portfolio& port, TimeFrame tf, size_t quantity, double confidence)  {

	if (confidence > 1 || confidence < 0) {

		Logger::logError("Confidence should be between 1 and 0.");
		return 0;
	}

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	std::sort(returns.begin(), returns.end(), std::greater<double>());
	std::vector<double> window{ returns.begin() + size_t(floor(returns.size() * confidence)) , returns.end() };
	
	//for (auto ret : window) {

	//	Logger::logDebug("Window: " + std::to_string(ret));
	//}

	return - (RiskStatistics::mean(window) * totalReturn(port));
}


double RiskEngine::parametricVaR(const Portfolio& port, TimeFrame tf, size_t quantity, ConfidenceLevel cl)  {

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	double z_value = RiskStatistics::zScores(cl);

	return - totalReturn(port) * (RiskStatistics::mean(returns) - (RiskStatistics::zScores(cl) * RiskStatistics::standardDeviation(returns)));
}


double RiskEngine::parametricShortfall(const Portfolio& port, TimeFrame tf, size_t quantity, ConfidenceLevel cl)  {

	double probability;
	switch (cl)
	{
	case ConfidenceLevel::NINETY_FIVE:

		probability = 0.95;
		break;

	case ConfidenceLevel::NINETY_NINE:

		probability = 0.99;
		break;

	case ConfidenceLevel::NINETY_NINE_FIVE:

		probability = 0.995;
		break;

	case ConfidenceLevel::NINET_NINE_NINE:

		probability = 0.999;
		break;

	default:
		throw "Invalid Confidence level.";
	}

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	double z_value = RiskStatistics::zScores(cl);

	return (RiskStatistics::mean(returns) + RiskStatistics::standardDeviation(returns) * (exp(-pow(z_value,2)/2) * 1/sqrt(2 * PI) * 1/(1-probability))) * totalReturn(port);
}


double RiskEngine::portfolioSharpeRatio(const Portfolio& port, TimeFrame tf, size_t quantity, double risk_free_rate)  {

	return (expectedReturn(port, tf) - risk_free_rate) / portfolioVolatility(port,tf);
}


double RiskEngine::assetCovariance(const Position& first, const Position& second, TimeFrame tf)  {

	return RiskStatistics::covariance(first.viewAsset()->historicData()->periodicReturns(tf), second.viewAsset()->historicData()->periodicReturns(tf));
}


double RiskEngine::assetCorrelation(const Position& first, const Position& second, TimeFrame tf)  {

	return RiskStatistics::correlation(first.viewAsset()->historicData()->periodicReturns(tf), second.viewAsset()->historicData()->periodicReturns(tf));
} 


std::vector<PositionRiskReport> RiskEngine::breakdown(const Portfolio& port, TimeFrame tf)  {

	std::vector<PositionRiskReport> breakdowns;
	for (auto& pos : port.viewPositions()) {

		breakdowns.push_back(analysePosition(*pos.second, tf));
	}

	return breakdowns;
}


