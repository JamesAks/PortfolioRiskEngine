#include "../include/RiskEngine.hpp"



RiskEngine::RiskEngine(RiskStatistics rstat) : stats{ rstat }{};


RiskReport RiskEngine::analysePortfolio(Portfolio port) {

	RiskReport report;
	for (Asset a : port.viewAssets()) {
		 
	}

	return report;
}


RiskReport RiskEngine::analyseAsset(Asset asset) {

	RiskReport report;
	report.assetID = asset.symbol();
	report.volatility = stats.variance(asset.historicData().prices);

	displayReport(report);

	return report;
}


void RiskEngine::displayReport(RiskReport report) {

	printf("Risk Report \n");
	printf("Asset ID : %s \n", report.assetID.c_str());
	printf("Volatility : %f \n", report.volatility);
}