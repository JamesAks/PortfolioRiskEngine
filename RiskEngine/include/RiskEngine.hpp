#ifndef RISK_ENGINE_CPP
#define RISK_ENGINE_CPP

#include <chrono>
#include <memory>
#include <string>
#include <vector>



class Asset;
class AssetReport;
class CovarianceMatrix;
class EfficientFrontier;
class EfficientFrontierPoint;
class MarketData;
class MarketDataStore;
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

	double total_intial_investment;

	double total_market_value;
	double expected_return;
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

	std::shared_ptr<EfficientFrontier> efficient_frontier;
};

struct AssetReport {

	double volatility;
	double expected_return;
	double net_present_value;
	std::chrono::year_month_day latest_valutaion_date;

};


struct PositionReport {

	
	double initial_investment;
	double market_value;
	double unrealised_gain;
	AssetReport asset_report;
	double total_market_value;
};


class RiskEngine {

	// Class for calculating risk metrics for domain types.

	private:

		// Stores a snapshot of market data due to the asynchronous behaviours.
		std::shared_ptr<const std::map<std::string, MarketData>> snapshot;

	public:

		RiskEngine();
		RiskEngine(std::shared_ptr<const std::map<std::string, MarketData>>);

	// Asset Analysis

		AssetReport analyseAsset(Asset&, TimeFrame) const;

		double assetCovariance(const Asset&, const Asset&, TimeFrame) const;

		double assetCorrelation(const Asset&, const Asset&, TimeFrame) const;

		double expectedAssetReturn(const Asset&, TimeFrame) const;


	// Position Analysis

		PositionReport analysePosition(const Position&, TimeFrame) const;

		double positionUnrealizedGains(const Position&) const;

		double positionMarketValue(const Position&) const;


	// Portfolio Analysis

		PortfolioReport analysePortfolio(const Portfolio&, TimeFrame) const;

		std::map<std::string, double> portfolioWeights(const Portfolio&) const;
	
		std::vector<double> portfolioPeriodicReturns(const Portfolio&, TimeFrame, size_t) const;

		double totalInitialInvestment(const Portfolio&) const;

		double totalMarketValue(const Portfolio&) const;

		double totalReturn(const Portfolio&) const;

		double expectedReturn(const Portfolio&, TimeFrame) const;

		double portfolioVolatility(const Portfolio&, TimeFrame) const;

		CovarianceMatrix computeCovarianceMatrix(const Portfolio&, TimeFrame) const;

		double historicalVaR(const Portfolio&, TimeFrame, size_t, double) const;

		double historicalShortfall(const Portfolio&, TimeFrame, size_t, double) const;

		double parametricVaR(const Portfolio&, TimeFrame, size_t, ConfidenceLevel) const;

		double parametricShortfall(const Portfolio&, TimeFrame, size_t, ConfidenceLevel) const;

		double portfolioSharpeRatio(const Portfolio&, TimeFrame, double) const;

		std::vector<double> expectedAssetReturns(const Portfolio&, TimeFrame) const;

		std::shared_ptr<EfficientFrontier> calculateEfficientFrontier(const Portfolio&, TimeFrame) const;

	// Utitility

		void updateSnapshot(std::shared_ptr<const std::map<std::string, MarketData>>);
};

#endif // !RISK_ENGINE_CPP