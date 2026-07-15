#include <catch2/catch_test_macros.hpp>

#include "GenericDataStore.hpp"
#include "CSVDataProvider.hpp"
#include "AlphaVantageProvider.hpp"



// ----- Factories -----



// ----- Tests ----- 

TEST_CASE("GenericDataStore is empty on creation.", "[MarketDataService]") {

	// Arrange

	// Act
	GenericDataStore sut;

	// Assert
	REQUIRE(sut.size() == 0);	
}


TEST_CASE("GenericDataStore can add market data for a valid ticker.", "[MarketDataService]") {

	// Arrange
	GenericDataStore sut(std::make_shared<CSVDataProvider>());

	// Act
	sut.addMarketData("AAPL");

	// Assert
	REQUIRE(sut.size() == 1);
}


TEST_CASE("GenericDataStore can remove market data for a valid ticker.", "[MarketDataService]") {

	// Arrange
	GenericDataStore sut(std::make_shared<CSVDataProvider>());
	sut.addMarketData("AAPL");

	// Act
	sut.removeMarketData("AAPL");
	
	// Assert
	REQUIRE(sut.size() == 0);
}
