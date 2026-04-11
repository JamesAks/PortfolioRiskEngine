#include "Statistic.hpp"
#include "Portfolio.hpp"



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
	double testCovariance = 0;
	double testCorrelation = 0;
	std::vector<AssetRiskReport> breakdowns = {};
};

// The class responsible for the risk calculations for example volatility and expected returns.
class RiskEngine {

	
	private:

		RiskStatistics stats;

	public:

		RiskEngine();
		RiskEngine(RiskStatistics);
		
		// Returns a report containing the riskmetrics of an asset.
		AssetRiskReport analyseAsset(Asset);

		// Returns a report containing the riskmetrics of a portfolio.
		PortfolioRiskReport analysePortfolio(Portfolio);

		// The changes in the price over a period of time.
		std::vector<double> periodicReturns(Asset);

		// The mean percentage change of the asset.
		double expectedReturn(Asset);

		// Total return of a portfolio. The sum total of the market values of each asset.
		double totalReturn(Portfolio);

		// The expected percentage return on investment
		double expectedReturn(Portfolio);

		// The risk of the portfolio using the variance-covariance formula.
		double portfolioVolatility(Portfolio);

		// The covaraince between two seperate assets.
		double assetCovariance(Asset, Asset);

		// The correlation between two assets using their covariances.
		double assetCorrelation(Asset, Asset);

		// A list containing anylysis of all the assets within a portfolio.
		std::vector<AssetRiskReport> breakdown(Portfolio);

};

// Methods for printing a portfolios risk report.
void displayPortfolioReport(PortfolioRiskReport);

// Method for printing an assets risk report.
void displayAssetReport(AssetRiskReport);