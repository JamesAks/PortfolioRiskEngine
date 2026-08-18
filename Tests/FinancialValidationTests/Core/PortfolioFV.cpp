#include <catch2/catch_test_macros.hpp>

#include "Portfolio.hpp"
#include "Position.hpp"
#include "MarketData.hpp"
#include "TimeSeries.hpp"
#include "Equities/Stock.hpp"



// ----- Factories -----

static Stock createStock(const std::string& name) {

	Stock test_stock{ name};

	return test_stock;
}

// ----- Tests -----

TEST_CASE("Portfolio calculates the correct total inital investment.", "[Core]") {

	// Arrange
	auto test_pos_one = std::make_shared<Position>("Test Position 1", 5, std::make_shared<Stock>(createStock("Test Stock 1")), 100, PositionType::LONG);
	auto test_pos_two = std::make_shared<Position>("Test Position 2", 5, std::make_shared<Stock>(createStock("Test Stock 2")), 200, PositionType::LONG);
	Portfolio sut{ "Test Portfolio" };

	sut.addPosition(test_pos_one);
	sut.addPosition(test_pos_two);

	// Act
	auto result = sut.viewTotalInvestment();

	// Assert
	REQUIRE(result == 1500.0);
}


