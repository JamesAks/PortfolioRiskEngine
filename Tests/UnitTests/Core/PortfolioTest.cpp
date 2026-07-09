#include <catch2/catch_test_macros.hpp>

#include "Portfolio.hpp"
#include "MarketData.hpp"
#include "TimeSeries.hpp"
#include "Stock.hpp"



// ----- Factories -----

static Stock createStock() {

	auto  lp = std::make_shared<LatestPrice>();
	auto  hd = std::make_shared<HistoricData>();

	Stock test_stock{"Test Stock", lp, hd };

	return test_stock;
}

// ----- Tests -----

TEST_CASE("Portfolio is empty on creation.", "[Core]") {

	// Arrange
	
	// Act
	Portfolio sut{ "Test1" };

	// Assert
	REQUIRE(sut.size() == 0);
}


TEST_CASE("Portfolio can add positions.", "[Core]") {

	// Arrange
	Position test_position{ "TestPosition", 100, std::make_shared<Stock>(createStock()), 120.0, PositionType::LONG};
	Portfolio sut{ "Test1" };

	// Act
	sut.addPosition(test_position);

	// Assert
	REQUIRE(sut.size() == 1);
}


TEST_CASE("Portfolio can remove positions.", "[Core]") {

	// Arrange
	Position test_position{ "TestPosition", 100, std::make_shared<Stock>(createStock()), 120.0, PositionType::LONG };
	Portfolio sut{ "Test1" };
	sut.addPosition(test_position);

	// Act
	sut.removePosition("TestPosition");

	// Assert
	REQUIRE(sut.size() == 0);
}


TEST_CASE("Portfolio ID can be changed", "[Core]") {

	// Arrange
	Portfolio sut{"TestP"};

	// Act
	sut.changeID("TestPosition");

	// Assert
	REQUIRE(sut.viewID() == "TestPosition");
}