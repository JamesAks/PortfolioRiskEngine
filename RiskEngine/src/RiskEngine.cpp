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

RiskEngine::RiskEngine(): snapshot{nullptr}{}

RiskEngine::RiskEngine(std::shared_ptr<const std::map<std::string, MarketData>> snap) : snapshot{snap} {}

AssetReport RiskEngine::analyseAsset(Asset& asset, TimeFrame tf) const {

	Logger::logInfo("Analysing Asset \"" + asset.symbol() + "\".");

	const MarketData& asset_market_data = snapshot->find(asset.symbol())->second;

	return {

		RiskCalculations::standardDeviation(asset_market_data.historic_data.periodicReturns(tf)),
		RiskCalculations::mean(asset_market_data.historic_data.periodicReturns(tf)),
		asset_market_data.latest_price.price()
	};
}

double RiskEngine::assetCovariance(const Asset& first, const Asset& second, TimeFrame tf) const {

	const HistoricData& historic_data_first = snapshot->find(first.symbol())->second.historic_data;
	const HistoricData& historic_data_second = snapshot->find(second.symbol())->second.historic_data;

	return RiskCalculations::covariance(historic_data_first.periodicReturns(tf), historic_data_second.periodicReturns(tf));
}


double RiskEngine::assetCorrelation(const Asset& first, const Asset& second, TimeFrame tf) const {

	const HistoricData& historic_data_first = snapshot->find(first.symbol())->second.historic_data;
	const HistoricData& historic_data_second = snapshot->find(second.symbol())->second.historic_data;

	return RiskCalculations::correlation(historic_data_first.periodicReturns(tf), historic_data_second.periodicReturns(tf));
}


double RiskEngine::expectedAssetReturn(const Asset& asset, TimeFrame tf) const {
	
	const HistoricData& historic_data = snapshot->find(asset.symbol())->second.historic_data;

	return RiskCalculations::mean(historic_data.periodicReturns(tf));
}


// Position Analysis

PositionReport RiskEngine::analysePosition(const Position& position, TimeFrame tf) const {

	Logger::logInfo("Analysing Position \"" + position.viewID() + "\".");

	auto asset_report = analyseAsset(*position.viewAsset(), tf);

	return {

		position.initialInvestment(),
		positionMarketValue(position),
		positionUnrealizedGains(position),
		asset_report,
		asset_report.net_present_value * position.viewQuantity()
	};
}


double RiskEngine::positionUnrealizedGains(const Position& position) const {

	double quantity = position.viewQuantity();

	if (position.viewPositionType() == PositionType::SHORT) {
		quantity *= -1;
	}

	const LatestPrice& latest_price = snapshot->find(position.viewAsset()->symbol())->second.latest_price;

	return (latest_price.price() - position.viewPriceBoughtAt()) * quantity;
}


double RiskEngine::positionMarketValue(const Position& position) const {

	const LatestPrice& latest_price = snapshot->find(position.viewAsset()->symbol())->second.latest_price;

	return latest_price.price() *position.viewQuantity();
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


PortfolioReport RiskEngine::analysePortfolio(const Portfolio& port, TimeFrame tf) const {

	// Due to asynchronous behaviour takes a copy of the portfolio as a sort of snapshot so that half way through calculations the 
	std::string portfolio_ID = port.viewID();
	Logger::logInfo("Analysing portfolio \"" + portfolio_ID + "\".");

	PortfolioReport report{

		totalMarketValue(port),
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
		parametricShortfall(port,tf, 100, ConfidenceLevel::NINETY_NINE_NINE),
		calculateEfficientFrontier(port,tf)
	};

	return report;
}

std::map<std::string, double> RiskEngine::portfolioWeights(const Portfolio& portfolio) const {

	std::map<std::string, double> weights;
	
	double total_market_value = totalMarketValue(portfolio);

	for (const auto& [name, position] : portfolio.viewPositions()) {

		weights.emplace(name, positionMarketValue(*position)/ total_market_value);
	}

	return weights;
}


std::vector<double> RiskEngine::portfolioPeriodicReturns(const Portfolio& portfolio, TimeFrame tf, size_t quantity) const{

	std::vector<double> returns;
	returns.resize(quantity - 1, 0);
  	auto weights = portfolioWeights(portfolio);

	for (const auto& [name, pos] : portfolio.viewPositions()) {
		
		const HistoricData & historic_data = snapshot->find(pos->viewAsset()->symbol())->second.historic_data;;
		const auto& pos_returns = historic_data.periodicReturns(tf);
		auto weight = weights.find(name);
		if (weight == weights.end()) { throw "Weight not found."; }

		for (int i = 0; i < quantity - 1; i++) {

			returns[i] += pos_returns[i] * weight->second;
		}
	}

	return returns;
}

double RiskEngine::totalMarketValue(const Portfolio& portfolio) const {

	double sum = 0;
	for (const auto& [name, position] : portfolio.viewPositions()) {

		sum += positionMarketValue(*position);
	}

	return sum;
}


double RiskEngine::totalReturn(const Portfolio& port) const {

	double sum = 0;
	for (auto& [name,pos] : port.viewPositions()) {

		const LatestPrice& latest_price = snapshot->find(pos->viewAsset()->symbol())->second.latest_price;
		sum += latest_price.price();
	}

	return sum;
}


double RiskEngine::expectedReturn(const Portfolio& portfolio, TimeFrame tf) const {

	auto ws = portfolioWeights(portfolio);

	if (ws.size() == 0) {
		return NULL;
	}

	double sum = 0;
	int i = 0;

	for (auto& [name, position] : portfolio.viewPositions()) {

		sum += ws.find(position->viewID())->second * expectedAssetReturn(*position->viewAsset(), tf);
		i++;
	}

	return sum;
}


double RiskEngine::portfolioVolatility(const Portfolio& portfolio, TimeFrame tf) const {

	std::vector<double> weights;
	for (const auto& [name, weight] : portfolioWeights(portfolio)) {

		weights.push_back(weight);
	}
	
	Eigen::MatrixXd cov_matrix = computeCovarianceMatrix(portfolio, tf).matrixData();
	Eigen::Map<Eigen::MatrixXd> weights_matrix(weights.data(), portfolio.size(), 1);
	double variance = (weights_matrix.transpose() * cov_matrix * weights_matrix).value();
	return sqrt(variance);
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


double RiskEngine::historicalVaR(const Portfolio& port, TimeFrame tf, size_t quantity, double confidence) const {

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


double RiskEngine::historicalShortfall(const Portfolio& port, TimeFrame tf, size_t quantity, double confidence) const {

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


double RiskEngine::parametricVaR(const Portfolio& port, TimeFrame tf, size_t quantity, ConfidenceLevel cl) const {

	auto returns = portfolioPeriodicReturns(port, tf, quantity);
	double z_value = RiskCalculations::zScores(cl);

	return - totalReturn(port) * (RiskCalculations::mean(returns) - (RiskCalculations::zScores(cl) * RiskCalculations::standardDeviation(returns)));
}


double RiskEngine::parametricShortfall(const Portfolio& port, TimeFrame tf, size_t quantity, ConfidenceLevel cl) const {

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


double RiskEngine::portfolioSharpeRatio(const Portfolio& port, TimeFrame tf, double risk_free_rate) const {

	return (expectedReturn(port, tf) - risk_free_rate) / portfolioVolatility(port,tf);
}




std::vector<double> RiskEngine::expectedAssetReturns(const Portfolio& portfolio, TimeFrame tf) const {

	std::vector<double> asset_returns;
	asset_returns.reserve(portfolio.size());

	for(auto& [name,position] : portfolio.viewPositions()){

		asset_returns.push_back(expectedAssetReturn(*position->viewAsset(), tf));
	}

	return asset_returns;
}


std::shared_ptr<EfficientFrontier> RiskEngine::calculateEfficientFrontier(const Portfolio& portfolio, TimeFrame tf) const {

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

	return std::make_shared<EfficientFrontier>(points);
}

// Utility
void RiskEngine::updateSnapshot(std::shared_ptr<const std::map<std::string, MarketData>> snap) { snapshot = snap; }



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


