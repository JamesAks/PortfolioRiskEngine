#include <catch2/catch_test_macros.hpp>

#include "Portfolio.hpp"
#include "Position.hpp"
#include "CSVDataProvider.hpp"
#include "GenericDataStore.hpp"
#include "TimeSeries.hpp"
#include "Equities/Stock.hpp"

// Factories

// Test

TEST_CASE("GenericDataStore automatically updates market data.", "[Integration]") {

	// Arrange

	CSVDataProvider market_data_provider;
	GenericDataStore sut{ &market_data_provider };

	sut.addMarketData("AAPL");

	// Act

	std::this_thread::sleep_for()

	// Assert
}