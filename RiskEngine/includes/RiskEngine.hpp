#include "Statistic.hpp"
#include "Portfolio.hpp"
#include "CovarianceMatrix.hpp"
#include "MarketDataManager.hpp"

struct PositionRiskReport {

	std::string position_ID = "";
	size_t quantity = 0;
	double price_bought_at = 0;
	double total_market_value = 0;
	double initial_investment = 0;
	double unrealised_gain = 0;

	std::string asset_ID = "";
	double market_value = 0;
	double volatility = 0;
	double expected_return = 0;
};


struct PortfolioRiskReport {

	std::string ID = "";
	double total_return = 0;
	double expectedReturn = 0;
	double volatitilty = 0;
	std::vector<PositionRiskReport> breakdowns = {};
	CovarianceMatrix cov_matrix;
};

// The class responsible for the risk calculations for example volatility and expected returns.
class RiskEngine {

	
	private:

		const MarketDataManager& market_data_manager;

	public:

		RiskEngine(MarketDataManager&);
		
		// Returns a report containing the riskmetrics of an asset.
		PositionRiskReport analysePosition(const Position&, TimeFrame) const;

		// Returns a report containing the riskmetrics of a portfolio.
		PortfolioRiskReport analysePortfolio(const Portfolio&, TimeFrame) const ;

		// The changes in the price over a period of time.
		std::vector<double>periodicReturns(const Position&, TimeFrame) const;

		// The mean percentage change of the asset.
		double expectedReturn(const Position&, TimeFrame) const;

		// Total return of a portfolio. The sum total of the market values of each asset.
		double totalReturn(const Portfolio&) const;

		// The expected percentage return on investment
		double expectedReturn(const Portfolio&, TimeFrame) const;

		// The risk of the portfolio using the variance-covariance formula.
		double portfolioVolatility(const Portfolio&, TimeFrame) const;

		// Computes the covariance matrix of a givenm portfolio.
		CovarianceMatrix computeCovarianceMatrix(const Portfolio&, TimeFrame) const;

		// The covaraince between two seperate assets.
		double assetCovariance(const Position&, const Position&, TimeFrame) const;

		// The correlation between two assets using their covariances.
		double assetCorrelation(const Position&, const Position&, TimeFrame) const;

		// A list containing anylysis of all the assets within a portfolio.
		std::vector<PositionRiskReport> breakdown(const Portfolio&, TimeFrame) const;

		
};
