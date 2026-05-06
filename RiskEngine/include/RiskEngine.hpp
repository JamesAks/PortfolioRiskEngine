#include "Statistic.hpp"
#include "Portfolio.hpp"
#include "CovarianceMatrix.hpp"
#include "MarketDataManager.hpp"



struct AssetRiskReport {

	std::string ID = "";
	double volatility = 0;
	double market_value = 0;
	double expected_return = 0;
};


struct PortfolioRiskReport {

	std::string ID = "";
	double total_return = 0;
	double expectedReturn = 0;
	double volatitilty = 0;
	std::vector<AssetRiskReport> breakdowns = {};
	CovarianceMatrix cov_matrix;
};

// The class responsible for the risk calculations for example volatility and expected returns.
class RiskEngine {

	
	private:

		MarketDataManager& market_data_manager;

	public:

		RiskEngine(MarketDataManager&);
		
		// Returns a report containing the riskmetrics of an asset.
		AssetRiskReport analyseAsset(Position, TimeFrame);

		// Returns a report containing the riskmetrics of a portfolio.
		PortfolioRiskReport analysePortfolio(Portfolio, TimeFrame tf);

		// The changes in the price over a period of time.
		std::vector<double>periodicReturns(Position, TimeFrame);

		// The mean percentage change of the asset.
		double expectedReturn(Position, TimeFrame);

		// Total return of a portfolio. The sum total of the market values of each asset.
		double totalReturn(Portfolio);

		// The expected percentage return on investment
		double expectedReturn(Portfolio, TimeFrame);

		// The risk of the portfolio using the variance-covariance formula.
		double portfolioVolatility(Portfolio, TimeFrame);

		// Computes the covariance matrix of a givenm portfolio.
		CovarianceMatrix computeCovarianceMatrix(Portfolio, TimeFrame);

		// The covaraince between two seperate assets.
		double assetCovariance(Position, Position, TimeFrame);

		// The correlation between two assets using their covariances.
		double assetCorrelation(Position, Position, TimeFrame);

		// A list containing anylysis of all the assets within a portfolio.
		std::vector<AssetRiskReport> breakdown(Portfolio, TimeFrame);

};
