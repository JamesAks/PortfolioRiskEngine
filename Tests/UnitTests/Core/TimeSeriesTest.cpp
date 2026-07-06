#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include "TimeSeries.hpp"

TEST_CASE("TimeSeries can store data", "[Portfolio]") {

	// Arrange

	auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(31));
	double price = 250;
	TimeSeries sut;

	// Act

	sut.addData(date, price);

	// Assert

	REQUIRE(sut.size() == 1 );
}