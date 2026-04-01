#include "Statistic.hpp"
#include "Portfolio.hpp"



struct RiskReport {

	std::string assetID;
	double volatility = 0;
};


class RiskEngine {

	// The class responsible for the risk calculations for example volatility and expected returns
	private:

		RiskStatistics stats;

	public:

		RiskEngine(RiskStatistics);
		RiskReport analysePortfolio(Portfolio);
		RiskReport analyseAsset(Asset);
		void displayReport(RiskReport); 
};
