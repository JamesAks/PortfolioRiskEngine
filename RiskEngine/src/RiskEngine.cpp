#include "Asset.hpp"
#include "CovarianceMatrix.hpp"
#include "EfficientFrontier.hpp"
#include "Logger.hpp"
#include "MarketData.hpp"
#include "Objectives.hpp"
#include "Optimizer.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "RiskEngine.hpp"
#include "RiskCalculations.hpp"
#include "Solvers/GMVPLagrangianSolver.hpp"
#include "Solvers/MVLagrangianSolver.hpp"
#include "TimeSeries.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>

// Asset Analysis

constexpr double PI = 3.14159265358979323846;

AssetReport RiskEngine::analyseAsset(Asset& asset, TimeFrame tf) {

	Logger::logInfo("Analysing Asset \"" + asset.symbol() + "\".");

	return {

		RiskCalculations::standardDeviation(asset.historicData()->periodicReturns(tf)),
		RiskCalculations::mean(asset.historicData()->periodicReturns(tf)),
		asset.NPV()
	};
}

double RiskEngine::assetCovariance(const Asset& first, const Asset& second, TimeFrame tf) {

	return RiskCalculations::covariance(first.historicData()->periodicReturns(tf), second.historicData()->periodicReturns(tf));
}


double RiskEngine::assetCorrelation(const Asset& first, const Asset& second, TimeFrame tf) {

	return RiskCalculations::correlation(first.historicData()->periodicReturns(tf), second.historicData()->periodicReturns(tf));
}


double RiskEngine::expectedAssetReturn(const Asset& pos, TimeFrame tf) { return RiskCalculations::mean(pos.historicData()->periodicReturns(tf)); }


// Position Analysis

PositionReport RiskEngine::analysePosition(const Position& position, TimeFrame tf) {

	Logger::logInfo("Analysing Position \"" + position.viewID() + "\".");

	auto asset_report = analyseAsset(*position.viewAsset(), tf);

	return {

		position.initialInvestment(),
		position.unrealizedGains(),
		asset_report,
		asset_report.net_present_value * position.viewQuantity()
	};
}

//PositionRiskReport RiskEngine::analysePosition(const Position& pos, TimeFrame tf)  {
//
//	Logger::logInfo("Analysing Position \"" + pos.viewID() + "\".");
//	std::string symbol = pos.viewID();
//	
//	PositionRiskReport report{
//
//		pos.viewID(),
//		pos.viewQuantity(),
//		pos.viewPriceBoughtAt(),
//		pos.marketValue(),
//		pos.initialInvestment(),
//		pos.unrealizedGains(),
//
//		pos.viewAsset()->symbol(),
//		pos.viewAsset()->NPV(),
//		expectedReturn(pos, tf),
//	};
//
//	return report;
//}


PortfolioReport RiskEngine::analysePortfolio(const Portfolio& port, TimeFrame tf)  {

	std::string portfolio_ID = port.viewID();
	Logger::logInfo("Analysing portfolio \"" + portfolio_ID + "\".");

	if (port.viewPositions().empty()) {

		Logger::logError("Can not anylse portfolio \"" + portfolio_ID + "\". No positions held in portfolio.");
		return {};
	}

	PortfolioReport report{

		totalReturn(port),
		expectedReturn(port,  tf),
		portfolioVolatility(port, tf),
		portfolioSharpeRatio(port, tf, 0.04),
		historicalVaR(port,tf, 100, 0.95),
		historicalVaR(port,tf, 100, 0.99),
		historicalVaR(port,tf, 100, 0.995),
		historicalVaR(port,tf, 100, 0.999),
		parametricVaR(port,tf, 100, ConfidenceLevel::NINETY_FIVE),
		parametricVaR(port,tf, 100, ConfidenceLevel::NINETY_NINE),
		parametricVaR(port,tf, 100, ConfidenceLevel::NINETY_NINE_FIVE),
		parametricVaR(port,tf, 100, ConfidenceLevel::NINETY_NINE_NINE),
		historicalShortfall(port,tf, 100, 0.95),
		historicalShortfall(port,tf, 100, 0.99),
		historicalShortfall(port,tf, 100, 0.995),
		historicalShortfall(port,tf, 100, 0.999),
		parametricShortfall(port,tf, 100, ConfidenceLevel::NINETY_FIVE),
		parametricShortfall(port,tf, 100, ConfidenceLevel::NINETY_NINE),
		parametricShortfall(port,tf, 100, ConfidenceLevel::NINETY_NINE_FIVE),
		parametricShortfall(port,tf, 100, ConfidenceLevel::NINETY_NINE_NINE)
	};

	return report;
}


std::vector<double> RiskEngine::portfolioPeriodicReturns(const Portfolio& port, TimeFrame tf, size_t quantity)  {

	std::vector<double> returns;
	returns.resize(quantity - 1, 0);
  	auto weights = port.weights();

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



double RiskEngine::totalReturn(const Portfolio& port)  {

	double sum = 0;
	for (auto& pos : port.viewPositions()) {

		sum += pos.second->marketValue();
	}

	return sum;
}


double RiskEngine::expectedReturn(const Portfolio& port, TimeFrame tf)  {

	auto ws = port.weights();

	if (ws.size() == 0) {
		return NULL;
	}

	double sum = 0;
	int i = 0;

	for (auto& [name, position] : port.viewPositions()) {

		sum += ws.find(position->viewID())->second * expectedAssetReturn(*position->viewAsset(), tf);
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

				cov_matrix(i, j) = assetCovariance(*pos.second->viewAsset(), *pos_2.second->viewAsset(), tf);
				j++;
				continue;
			}

			double covariance = assetCovariance(*pos.second->viewAsset(), *pos_2.second->viewAsset(), tf);
			
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

	return - (RiskCalculations::mean(window) * totalReturn(port));
}


double RiskEngine::parametricVaR(const Portfolio& port, TimeFrame tf, size_t quantity, ConfidenceLevel cl)  {

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	double z_value = RiskCalculations::zScores(cl);

	return - totalReturn(port) * (RiskCalculations::mean(returns) - (RiskCalculations::zScores(cl) * RiskCalculations::standardDeviation(returns)));
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

	case ConfidenceLevel::NINETY_NINE_NINE:

		probability = 0.999;
		break;

	default:
		throw "Invalid Confidence level.";
	}

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	double z_value = RiskCalculations::zScores(cl);

	return (RiskCalculations::mean(returns) + RiskCalculations::standardDeviation(returns) * (exp(-pow(z_value,2)/2) * 1/sqrt(2 * PI) * 1/(1-probability))) * totalReturn(port);
}


double RiskEngine::portfolioSharpeRatio(const Portfolio& port, TimeFrame tf, double risk_free_rate)  {

	return (expectedReturn(port, tf) - risk_free_rate) / portfolioVolatility(port,tf);
}




std::vector<double> RiskEngine::expectedAssetReturns(const Portfolio& portfolio, TimeFrame tf) {

	std::vector<double> asset_returns;
	asset_returns.reserve(portfolio.size());

	for(auto& [name,position] : portfolio.viewPositions()){

		asset_returns.push_back(expectedAssetReturn(*position->viewAsset(), tf));
	}

	return asset_returns;
}


EfficientFrontier RiskEngine::calculateEfficientFrontier(const Portfolio& portfolio, TimeFrame tf) {

	Optimizer<MinimiseVolatility, MVLagrangianSolver> optimizer;
	std::array<EfficientFrontierPoint,50> points;
	

	auto cov_matrix = computeCovarianceMatrix(portfolio, tf);
	auto asset_returns = expectedAssetReturns(portfolio, tf);
	
	// Find Global Minimum Variance Portfolio to start and produce a maximum "feasible" return.

	Optimizer<MinimiseVolatility, GMVPLagrangianSolver> gmv_optimiser;
	auto solution = gmv_optimiser.optimise(asset_returns, cov_matrix);
	
	auto min_return = RiskCalculations::expectedReturn(solution, asset_returns);
	auto max_return = *std::max_element(asset_returns.begin(), asset_returns.end());

	for (auto returns : asset_returns) {

		Logger::logDebug("Asset return:" + std::to_string(returns));
	}

	Logger::logDebug("Minimum Return - " + std::to_string(min_return));
	Logger::logDebug("Maximum Return - " + std::to_string(max_return));


	// Construct the poortfolios/points for the efficient frontier.
	double target = min_return;

	for (int i = 0; i < 50;i++) {

		optimizer.viewSolver().changeTargetReturn(target);

		points[i] = {

			optimizer.optimise(asset_returns, cov_matrix),
			target,
			optimizer.currentScore()
	
		};

		Logger::logDebug("Volatility for target return " + std::to_string(target) + ":" + std::to_string(points[i].volatitity));

		target += (max_return - min_return)/50.0;
	}

	EfficientFrontier efficient_frontier{ points };
	return efficient_frontier;
}



//std::vector<PositionRiskReport> RiskEngine::breakdown(const Portfolio& port, TimeFrame tf)  {
//
//	std::vector<PositionRiskReport> breakdowns;
//	for (auto& pos : port.viewPositions()) {
//
//		breakdowns.push_back(analysePosition(*pos.second, tf));
//	}
//
//	return breakdowns;
//}


