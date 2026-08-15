#ifndef RISK_ENGINE_CPP
#define RISK_ENGINE_CPP

#include <memory>
#include <string>
#include <vector>


class Asset;
class AssetReport;
class CovarianceMatrix;
class EfficientFrontier;
class EfficientFrontierPoint;
class Position;
class Portfolio;
enum class ConfidenceLevel;
enum class TimeFrame;

struct PositionRiskReport {

	std::string position_ID;
	size_t quantity;
	double price_bought_at;
	double total_market_value;
	double initial_investment;
	double unrealised_gain;

	std::string asset_ID;
	double market_value;
	double volatility;
	double expected_return;
};

struct PortfolioReport {

	double total_return;
	double expectedReturn;
	double volatitilty;
	double sharpe_ratio;

	// Value at Risk (VaR)
	double historical_VaR_95;
	double historical_VaR_99;
	double historical_VaR_995;
	double historical_VaR_999;

	double parametric_VaR_95;
	double parametric_VaR_99;
	double parametric_VaR_995;
	double parametric_VaR_999;

	// Shortfall
	double historical_shortfall_95;
	double historical_shortfall_99;
	double historical_shortfall_995;
	double historical_shortfall_999;

	double parametric_shortfall_95;
	double parametric_shortfall_99;
	double parametric_shortfall_995;
	double parametric_shortfall_999;
};

struct AssetReport {

	double volatility;
	double expected_return;
	double net_present_value;
};


struct PositionReport {

	double initial_investment;
	double unrealised_gain;
	AssetReport asset_report;
	double total_market_value;
};


namespace RiskEngine {

	// Asset Analysis

		AssetReport analyseAsset(Asset&, TimeFrame);

		double assetCovariance(const Asset&, const Asset&, TimeFrame);

		double assetCorrelation(const Asset&, const Asset&, TimeFrame);

		double expectedAssetReturn(const Asset&, TimeFrame);
	

	// Position Analysis

		PositionReport analysePosition(const Position&, TimeFrame);


	// Portfolio Analysis

		PortfolioReport analysePortfolio(const Portfolio&, TimeFrame);
	
		std::vector<double> portfolioPeriodicReturns(const Portfolio&, TimeFrame, size_t);

		double totalReturn(const Portfolio&);

		double expectedReturn(const Portfolio&, TimeFrame);

		double portfolioVolatility(const Portfolio&, TimeFrame);

		CovarianceMatrix computeCovarianceMatrix(const Portfolio&, TimeFrame);

		double historicalVaR(const Portfolio&, TimeFrame, size_t, double);

		double historicalShortfall(const Portfolio&, TimeFrame, size_t, double);

		double parametricVaR(const Portfolio&, TimeFrame, size_t, ConfidenceLevel);

		double parametricShortfall(const Portfolio&, TimeFrame, size_t, ConfidenceLevel);

		double portfolioSharpeRatio(const Portfolio&, TimeFrame, double);

		std::vector<double> expectedAssetReturns(const Portfolio&, TimeFrame);

		EfficientFrontier calculateEfficientFrontier(const Portfolio&, TimeFrame);
};

#endif // !RISK_ENGINE_CPP