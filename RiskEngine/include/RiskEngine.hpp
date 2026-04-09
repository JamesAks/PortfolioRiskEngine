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
	double overallRisk = 0;
	std::vector<AssetRiskReport> breakdowns = {};
};


class RiskEngine {

	// The class responsible for the risk calculations for example volatility and expected returns
	private:

		RiskStatistics stats;

	public:

		RiskEngine();
		RiskEngine(RiskStatistics);
		
		AssetRiskReport analyseAsset(Asset);
		std::vector<double> returns(Asset);
		double expectedReturn(Asset);

		PortfolioRiskReport analysePortfolio(Portfolio);
		double totalReturn(Portfolio);
		double expectedReturn(Portfolio);
		double portfolioVolatility(Portfolio);
		double overallRisk(Portfolio);
		std::vector<AssetRiskReport> breakdown(Portfolio);

};

void displayPortfolioReport(PortfolioRiskReport);
void displayAssetReport(AssetRiskReport);