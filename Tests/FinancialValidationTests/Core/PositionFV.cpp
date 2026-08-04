#include <catch2/catch_test_macros.hpp>

#include <chrono>

#include "MarketData.hpp"
#include "Position.hpp"
#include "Equities/Stock.hpp"
#include "TimeSeries.hpp"



// ----- Factories -----

static TimeSeries createTimeSeries(int n) {

	TimeSeries ts;
	std::vector<std::chrono::year_month_day> dates;
	for (int i = 0; i < n; i++) {

		ts.addData(std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(n + 1)), (i * 50) + 100);
	}

	return ts;
}


static HistoricData createHistoricData(int n) {

	return {createTimeSeries(n),createTimeSeries(n), createTimeSeries(n) };
}

// ----- Tests -----

TEST_CASE("Position calculates the correct unrealized gains", "[Core]") {

	// Arrange
	auto hd = createHistoricData(5);
	LatestPrice lp{ 300, std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(6)) };
	
	Stock st{

		"AAPL",
		std::make_shared<LatestPrice>(lp),
		std::make_shared< HistoricData>(hd)
	};

	Position sut{ "AAPL Stock", 100, std::make_shared<Stock>(st), 100, PositionType::LONG };

	
	// Act
	double result = sut.unrealizedGains();

	// Assert
	REQUIRE(result == 20000);
}