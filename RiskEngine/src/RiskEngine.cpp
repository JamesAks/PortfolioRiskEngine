#include "../include/RiskEngine.hpp"



RiskEngine::RiskEngine(){};
RiskEngine::RiskEngine(RiskStatistics rstat) : stats{ rstat }{};


AssetRiskReport RiskEngine::analyseAsset(Asset asset) {

	AssetRiskReport report;
	report.ID = asset.symbol();
	report.volatility = stats.variance(asset.historicData().prices);
	report.market_value = asset.marketValue();
	report.expected_return = expectedReturn(asset);

	return report;
}

std::vector<double> RiskEngine::periodicReturns(Asset asset) {

	// Returns the percentage increas
	std::vector<double> asset_returns;
	std::vector<double> ps = asset.historicData().prices;

	for (size_t i = 1; i < ps.size(); i++) {

		asset_returns.push_back((ps[i] - ps[i - 1]) / ps[i - 1]);
	}

	return asset_returns;
}

double RiskEngine::expectedReturn(Asset asset) { return stats.mean(periodicReturns(asset)); }


PortfolioRiskReport RiskEngine::analysePortfolio(Portfolio port) {

	PortfolioRiskReport report;

	report.ID = port.viewName();
	report.total_return = totalReturn(port);
	report.expectedReturn = expectedReturn(port);
	report.volatitilty = portfolioVolatility(port);
	report.breakdowns = breakdown(port);
	report.testCovariance = assetCovariance(port.viewAssets()[0], port.viewAssets()[1]);
	report.testCorrelation = assetCorrelation(port.viewAssets()[0], port.viewAssets()[1]);
	
	return report;
}


double RiskEngine::totalReturn(Portfolio port) {

	double sum = 0;
	for (Asset a : port.viewAssets()) {

		sum += a.marketValue();
	}

	return sum;
}


double RiskEngine::expectedReturn(Portfolio port) {


	auto ws = port.weights();
	double sum = 0;

	for (int i = 0; i < ws.size(); i++) {

		sum += ws[i] * expectedReturn(port.viewAssets()[i]);
	}

	return sum;
}


double RiskEngine::portfolioVolatility(Portfolio port) {

	return 0;
}


double RiskEngine::assetCovariance(Asset first, Asset second) {

	// Covariance is how to 
	std::vector<double> f_returns = periodicReturns(first);
	std::vector<double> s_returns = periodicReturns(second);

	return stats.covariance(f_returns, s_returns);
}


double RiskEngine::assetCorrelation(Asset first, Asset second) {

	// Correlation is the relationship between 2 assets.
	std::vector<double> f_returns = periodicReturns(first);
	std::vector<double> s_returns = periodicReturns(second);

	return stats.correlation(f_returns, s_returns);

}


std::vector<AssetRiskReport> RiskEngine::breakdown(Portfolio port) {

	std::vector<AssetRiskReport> breakdowns;
	for (Asset a : port.viewAssets()) {

		breakdowns.push_back(analyseAsset(a));
	}

	return breakdowns;
}


void displayPortfolioReport(PortfolioRiskReport report) {

	printf("Portfolio ID : %s \n", report.ID.c_str());
	printf("Total Return : %f \n", report.total_return);
	printf("Expected Return : %f \n", report.expectedReturn);
	printf("Volatility : %f \n", report.volatitilty);
	printf("Test Covariance : %f \n ", report.testCovariance);
	printf("Test Correlation : %f \n \n", report.testCorrelation);

	printf("Asset Breakdown ---- Total Number of Assets : %zu ---- \n", report.breakdowns.size());

	for (int i = 0; i < report.breakdowns.size(); i++) {

		printf("Asset %d \n", i + 1);
		displayAssetReport(report.breakdowns[i]);
		printf("\n");
	}
}


void displayAssetReport(AssetRiskReport report) {

	printf("Asset ID : %s \n", report.ID.c_str());
	printf("Volatility : %f \n", report.volatility);
	printf("Market Value : %f \n", report.market_value );
	printf("Expected Return : %f \n", report.expected_return);
}


//void RiskEngine::displayReport(RiskReport report) {
//
//	printf("Risk Report \n");
//	printf("Asset ID : %s \n", report.assetID.c_str());
//	printf("Asset Market Value : %f \n", report.marketValue);
//	printf("Volatility : %f \n", report.volatility);
//	printf("")
//}