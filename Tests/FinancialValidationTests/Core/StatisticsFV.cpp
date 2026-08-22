#include <catch2/catch_test_macros.hpp>

#include "RiskCalculations.hpp"



TEST_CASE("RiskCalculations calculates the correct mean for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 400 };

	// Act
	auto sut = RiskCalculations::mean(test_prices);
	
	// Assert
	REQUIRE(sut == 250); 
}


TEST_CASE("RiskCalculations calculates the correct returns for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 450 };

	// Act
	auto sut = RiskCalculations::periodicReturns(test_prices);

	// Assert
	REQUIRE(sut == std::vector<double>{1, 0.5, 0.5});
}


TEST_CASE("RiskCalculations calculates the correct standard deviation for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 400 };

	// Act
	auto sut = RiskCalculations::standardDeviation(test_prices);

	// Assert
	REQUIRE(sut == 50 * sqrt(5));
}


TEST_CASE("RiskCalculations calculates the correct variance for a series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices{ 100, 200, 300, 400 };

	// Act
	auto sut = RiskCalculations::variance(test_prices);

	// Assert
	REQUIRE(sut == 12500);
}


TEST_CASE("RiskCalculations calculates the correct covariance for two different series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices_one{ 100, 200, 300, 400 };
	std::vector<double> test_prices_two{ 100, 300, 500, 800 };

	// Act
	auto sut = RiskCalculations::covariance(test_prices_one, test_prices_two);

	// Assert
	REQUIRE(sut == 28750);
}


TEST_CASE("RiskCalculations calculates the correct correlation for two different series of prices.", "[Core]") {

	// Arrange
	std::vector<double> test_prices_one{ 100, 200, 300, 400 };
	std::vector<double> test_prices_two{ 100, 300, 500, 800 };

	// Act
	auto sut = RiskCalculations::correlation(test_prices_one, test_prices_two);

	// Assert
	REQUIRE(sut == 0.99437671268436889);
}


