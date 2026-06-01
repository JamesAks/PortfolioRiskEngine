#ifndef RISK_ENGINE_CPP
#define RISK_ENGINE_CPP

#include "CovarianceMatrix.hpp"

#include <string>
#include <vector>



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
	std::vector<PositionRiskReport> breakdowns;
	CovarianceMatrix cov_matrix;
};


class RiskEngine {

	// The class responsible for the risk calculations.
	
	private:

		// std::map<std::string, std::vector<double>> returnsCache;
	

	public:

		RiskEngine();
		
		// Returns a report containing the riskmetrics of an asset.
		PositionRiskReport analysePosition(const Position&, TimeFrame) const;

		// Returns a report containing the riskmetrics of a portfolio.
		PortfolioRiskReport analysePortfolio(const Portfolio&, TimeFrame) const ;

		// Returns the percentage periodic change in price of the asset.
		std::vector<double>periodicReturns(const Position&, TimeFrame) const;

		// The mean percentage change of the asset.
		double expectedReturn(const Position&, TimeFrame) const;

		// Returns total return of a portfolio. The sum total of the market values of each asset.
		double totalReturn(const Portfolio&) const;

		// Returns the expected percentage return of asset.
		double expectedReturn(const Portfolio&, TimeFrame) const;

		// Returns the risk of the portfolio using the variance-covariance formula.
		double portfolioVolatility(const Portfolio&, TimeFrame) const;

		// Computes the covariance matrix of a givenm portfolio.
		CovarianceMatrix computeCovarianceMatrix(const Portfolio&, TimeFrame) const;

		// Returns the covaraince between two seperate assets.
		double assetCovariance(const Position&, const Position&, TimeFrame) const;

		// Returns the correlation between two assets using their covariances.
		double assetCorrelation(const Position&, const Position&, TimeFrame) const;

		// Returns a list containing anylysis of all the assets within a portfolio.
		std::vector<PositionRiskReport> breakdown(const Portfolio&, TimeFrame) const;	
};

#endif // !RISK_ENGINE_CPP
