#include <catch2/catch_test_macros.hpp>

#include "Position.hpp"
#include "Equities/Stock.hpp"


// ----- Factories -----

static Stock createStock(const std::string& name) {

	Stock test_stock{ name };

	return test_stock;
}

// ----- Tests -----

TEST_CASE("Position calculates the correct inital investment.", "[Core]") {

	// Arrange
	auto test_pos = std::make_shared<Position>("Test Position 1", 5, std::make_shared<Stock>(createStock("Test Stock 1")), 100, PositionType::LONG);


	// Act
	auto result = test_pos->initialInvestment();

	// Assert
	REQUIRE(result == 500.0);
}