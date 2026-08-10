#include <catch2/catch_test_macros.hpp>
#include "MarketData.hpp"
#include "RiskEngine.hpp"
#include "Logger.hpp"
#include "Portfolio.hpp"
#include "Position.hpp"
#include "Equities/Stock.hpp"
#include "TimeSeries.hpp"



static TimeSeries createTimeSeries(int n) {

	TimeSeries ts;
	std::vector<std::chrono::year_month_day> dates;
	for (int i = 0; i < n; i++) {

		ts.addData(std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(n + 1)), (i * 50) + 100);
	}

	return ts;
}


static HistoricData createHistoricData(int n) {

	return { createTimeSeries(n),createTimeSeries(n), createTimeSeries(n) };
}


TEST_CASE("RiskEngine can calculate an efficient frontier.", "[RiskEngine]") {

	// Arrange
	auto hd = createHistoricData(5);
	LatestPrice lp{ 300, std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(6)) };

	Stock st{

		"AAPL",
		std::make_shared<LatestPrice>(lp),
		std::make_shared< HistoricData>(hd)
	};

	Position pos{ "AAPL Stock", 100, std::make_shared<Stock>(st), 100, PositionType::LONG };

	Portfolio port{ "TestPortfolio" };
	port.addPosition(std::make_shared<Position>("AAPL Stock", 100, std::make_shared<Stock>(st), 100, PositionType::LONG));
	port.addPosition(std::make_shared<Position>("AAPL Stock2", 100, std::make_shared<Stock>(st), 100, PositionType::LONG));

	// Act
	auto sut = RiskEngine::calculateEfficientFrontier(port, TimeFrame::DAILY);

	// Assert

	//for (int i = 0; i < 19; i++) {

	//	Logger::logInfo("Point " + std::to_string(i));
	//	Logger::logInfo("Volatility" + std::to_string(sut[i].volatitity));
	//	Logger::logInfo("Return: " + std::to_string(sut[i].expected_return));

	//	for (auto& weight : sut[i].weights) {

	//		Logger::logInfo("Weights: " + std::to_string(weight));
	//	}
	//}
	REQUIRE(sut.size() == 19);



}