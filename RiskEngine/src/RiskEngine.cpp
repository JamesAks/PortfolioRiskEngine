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

std::vector<double> RiskEngine::returns(Asset asset) {

	std::vector<double> asset_returns;
	std::vector<double> ps = asset.historicData().prices;

	for (size_t i = 1; i < ps.size(); i++) {

		asset_returns.push_back((ps[i] - ps[i - 1]) / ps[i - 1]);
	}

	return asset_returns;
}

double RiskEngine::expectedReturn(Asset asset) { return stats.mean(returns(asset)); }


PortfolioRiskReport RiskEngine::analysePortfolio(Portfolio port) {

	PortfolioRiskReport report;

	report.ID = port.viewName();
	report.total_return = totalReturn(port);
	report.expectedReturn = expectedReturn(port);
	report.volatitilty = portfolioVolatility(port);
	report.overallRisk = overallRisk(port);
	report.breakdowns = breakdown(port);
	
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


double RiskEngine::overallRisk(Portfolio port) {

	return 0;
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
	printf("Overall Risk : %f \n", report.overallRisk);

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