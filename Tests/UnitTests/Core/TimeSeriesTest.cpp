#include <catch2/catch_test_macros.hpp>

#include <chrono>
#include <vector>

#include "TimeSeries.hpp"



// ----- Factories -----
static std::vector<std::chrono::year_month_day> createDates(int n = 5) {

	std::vector<std::chrono::year_month_day> dates;
	for (int i = 1; i < n; i++) {

		auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(n));
		dates.push_back(date);
	}

	return dates;
}

static std::vector<double> createPrices(int n = 5) {

	std::vector<double> prices;
	for (int i = 0; i < n; i++) {

		prices.push_back((i * 50) + 100);
	}

	return prices;
}


// ----- Tests -----

TEST_CASE("TimeSeries starts empty by default.", "[Core]") {

	// Arrange

	TimeSeries sut;

	// Act

	// Assert

	REQUIRE(sut.size() == 0);

}


TEST_CASE("TimeSeries can add data.", "[Core]") {

	// Arrange

	auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(31));
	double price = 250;
	TimeSeries sut;

	// Act

	sut.addData(date, price);

	// Assert

	REQUIRE(sut.size() == 1 );
}


TEST_CASE("TimeSeries automatically calculates returns on creation.", "[Core]") {

	// Arrange

	auto dates = createDates(5);
	auto prices = createPrices(5);

	// Act

	TimeSeries sut(dates, prices);

	// Assert

	REQUIRE(sut.returns().size() == 4);
}


TEST_CASE("TimeSeries automatically calculates returns on adding new data", "[Core]") {

	// Arrange

	auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(31));

	auto dates = createDates(5);
	auto prices = createPrices(5);

	TimeSeries sut(dates, prices);

	// Act

	sut.addData(date, 260);

	// Assert

	REQUIRE(sut.returns().size() == 5);
}







