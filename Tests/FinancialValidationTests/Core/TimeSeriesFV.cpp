#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <vector>

#include "TimeSeries.hpp"



// ----- Factories -----
static std::vector<std::chrono::year_month_day> createDates(int n) {

	if (n > 31) { throw("Number of days above 31."); }

	std::vector<std::chrono::year_month_day> dates;
	for (int i = 0; i < n; i++) {

		auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(n+1));
		dates.push_back(date);
	}

	return dates;
}

static std::vector<double> createPrices(int n) {

	std::vector<double> prices;
	for (int i = 0; i < n; i++) {

		prices.push_back((i * 50) + 100);
	}

	return prices;
}


// ----- Tests -----

TEST_CASE("TimeSeries generates the correct returns for a series of prices on creation.", "[Core]") {

	// Arrange
	auto dates = createDates(4);
	auto prices = createPrices(4);

	// Act
	TimeSeries sut(dates,prices);

	// Assert
	REQUIRE(sut.returns() == std::vector<double>{50.0 / 100, 50.0 / 150, 50.0 / 200});

}

TEST_CASE("TimeSeries generates the correct returns for a series of prices when a price is added", "[Core]") {

	// Arrange
	auto dates = createDates(3);
	auto prices = createPrices(3);

	auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(4));
	double price = 260;

	TimeSeries sut(dates, prices);


	// Act
	sut.addData(date, price);

	// Assert

	REQUIRE(sut.returns() == std::vector<double>{50.0 / 100, 50.0 / 150, 50.0 / 200});


}