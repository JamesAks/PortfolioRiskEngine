#include <catch2/catch_test_macros.hpp>

#include "AlphaVantageProvider.hpp"




const char* API_KEY = getenv("ALPHA_VANTAGE_API_KEY");

TEST_CASE("Provider provides a timeseries for a valid ticker.", "[MarketDataService]") {

	// Arrange
	AlphaVantageProvider sut(API_KEY);

	// Act
	auto result = sut.periodicData("AAPL", TimeFrame::DAILY, 100);

	// Assert
	REQUIRE(result.request_error == RequestError::NONE);
	REQUIRE(result.time_series.value().size() == 100);
}


TEST_CASE("Provider provides a latest price for a valid ticker.", "[MarketDataService]") {

	// Arrange
	AlphaVantageProvider sut(API_KEY);

	// Act
	auto result = sut.latestPrice("AAPL");

	// Assert

	REQUIRE(result.request_error == RequestError::NONE);
	REQUIRE(result.price.price() != NULL);
}



