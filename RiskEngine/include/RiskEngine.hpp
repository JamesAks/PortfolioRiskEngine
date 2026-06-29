#ifndef RISK_ENGINE_CPP
#define RISK_ENGINE_CPP

#include "CovarianceMatrix.hpp"


#include <string>
#include <vector>


enum class ConfidenceLevel;
enum class TimeFrame;
class Position;
class Portfolio;


struct PositionRiskReport {

	std::string position_ID;
	size_t quantity;
	double price_bought_at;
	double total_market_value;
	double initial_investment;
	double unrealised_gain;

	std::string asset_ID ;
	double market_value;
	double volatility ;
	double expected_return ;

};


struct PortfolioRiskReport {

	std::string ID;
	double total_return;
	double expectedReturn;
	double volatitilty;
	double historical_VaR;
	double historical_shortfall;
	double parametric_VaR;
	double parametric_shortfall;
	double sharpe_ratio;
	std::vector<PositionRiskReport> breakdowns;
	CovarianceMatrix cov_matrix;

};


namespace RiskEngine  {
	
		// Returns a report containing the riskmetrics of an asset.
		PositionRiskReport analysePosition(const Position&, TimeFrame);

		// Returns a report containing the riskmetrics of a portfolio.
		PortfolioRiskReport analysePortfolio(const Portfolio&, TimeFrame);

		// Returns the daily returns of a portfolio
		std::vector<double> portfolioPeriodicReturns(const Portfolio&, TimeFrame, size_t);

		// The mean percentage change of the asset.
		double expectedReturn(const Position&, TimeFrame);

		// Returns total return of a portfolio. The sum total of the market values of each asset.
		double totalReturn(const Portfolio&);

		// Returns the expected percentage return of asset.
		double expectedReturn(const Portfolio&, TimeFrame) ;

		// Returns the risk of the portfolio using the variance-covariance formula.
		double portfolioVolatility(const Portfolio&, TimeFrame);

		// Computes the covariance matrix of a givenm portfolio.
		CovarianceMatrix computeCovarianceMatrix(const Portfolio&, TimeFrame);

		double historicalVaR(const Portfolio&, TimeFrame, size_t, double);

		double historicalShortfall(const Portfolio&, TimeFrame, size_t, double);

		// Note: Only covers the case where the portfolios returns follow a normal distribution
		double parametricVaR(const Portfolio&, TimeFrame, size_t, ConfidenceLevel);

		double parametricShortfall(const Portfolio&, TimeFrame, size_t, ConfidenceLevel);

		double portfolioSharpeRatio(const Portfolio&,TimeFrame, size_t, double);

		// Returns the covaraince between two seperate assets.
		double assetCovariance(const Position&, const Position&, TimeFrame);

		// Returns the correlation between two assets using their covariances.
		double assetCorrelation(const Position&, const Position&, TimeFrame);

		// Returns a list containing anylysis of all the assets within a portfolio.
		std::vector<PositionRiskReport> breakdown(const Portfolio&, TimeFrame);	
};

#endif // !RISK_ENGINE_CPP

