#include <catch2/catch_test_macros.hpp>

#include "Portfolio.hpp"
#include "MarketData.hpp"
#include "TimeSeries.hpp"
#include "Equities/Stock.hpp"



// ----- Factories -----

static Stock createStock(const std::string& name, double latest_price) {

	auto  lp = std::make_shared<LatestPrice>( latest_price, std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(1)));
	auto  hd = std::make_shared<HistoricData>();

	Stock test_stock{ name, lp, hd };

	return test_stock;
}

// ----- Tests -----

TEST_CASE("Portfolio calculates the correct total market value of its positions.", "[Core]") {

	// Arrange
	Position test_pos_one{ "Test Position 1", 100, std::make_shared<Stock>(createStock("Test Stock 1", 100.0)), 100, PositionType::LONG };
	Position test_pos_two{ "Test Position 2", 100, std::make_shared<Stock>(createStock("Test Stock 2", 200.0)), 100, PositionType::LONG };

	Portfolio sut{ "Test Portfolio" };

	sut.addPosition(test_pos_one);
	sut.addPosition(test_pos_two);

	// Act
	auto result = sut.totalMarketValue();

	// Assert
	REQUIRE(result == 30000.0);
}


TEST_CASE("Portfolio calculates the correct weights for its positions.", "[Core]") {

	// Arrange
	Position test_pos_one{ "Test Position 1", 100, std::make_shared<Stock>(createStock("Test Stock 1", 100.0)), 100, PositionType::LONG };
	Position test_pos_two{ "Test Position 2", 100, std::make_shared<Stock>(createStock("Test Stock 2", 300.0)), 100, PositionType::LONG };

	Portfolio sut{ "Test Portfolio" };

	sut.addPosition(test_pos_one);
	sut.addPosition(test_pos_two);

	// Act
	auto weights = sut.weights();

	// Assert
	REQUIRE(weights["Test Position 1"] == 0.25);
	REQUIRE(weights["Test Position 2"] == 0.75);
}


