#include <catch2/catch_test_macros.hpp>

#include "Statistics.hpp"



TEST_CASE("Statistics calculates the correct mean for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 400 };

	// Act
	auto sut = RiskStatistics::mean(test_prices);
	
	// Assert
	REQUIRE(sut == 250); 
}


TEST_CASE("Statistics calculates the correct returns for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 450 };

	// Act
	auto sut = RiskStatistics::periodicReturns(test_prices);

	// Assert
	REQUIRE(sut == std::vector<double>{1, 0.5, 0.5});
}


TEST_CASE("Statistics calculates the correct standard deviation for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 400 };

	// Act
	auto sut = RiskStatistics::standardDeviation(test_prices);

	// Assert
	REQUIRE(sut == 50 * sqrt(5));
}


TEST_CASE("Statistics calculates the correct variance for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 400 };

	// Act
	auto sut = RiskStatistics::variance(test_prices);

	// Assert
	REQUIRE(sut == 12500);
}


TEST_CASE("Statistics calculates the correct covariance for two different series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices_one{ 100, 200, 300, 400 };
	std::vector<double> test_prices_two{ 100, 300, 500, 800 };

	// Act
	auto sut = RiskStatistics::covariance(test_prices_one, test_prices_two);

	// Assert
	REQUIRE(sut == 28750);
}


TEST_CASE("Statistics calculates the correct correlation for two different series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices_one{ 100, 200, 300, 400 };
	std::vector<double> test_prices_two{ 100, 300, 500, 800 };

	// Act
	auto sut = RiskStatistics::correlation(test_prices_one, test_prices_two);

	// Assert
	REQUIRE(sut == 0.99437671268436889);
}


