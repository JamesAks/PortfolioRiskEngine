#include <catch2/catch_test_macros.hpp>

#include<chrono>

#include "MarketData.hpp"



// ----- Factories -----

static std::vector<std::chrono::year_month_day> createDates(int n) {

	std::vector<std::chrono::year_month_day> dates;
	for (int i = 0; i < n; i++) {

		auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(n + 1));
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


// ----- HistoricData Tests -----

TEST_CASE("HistoricData TimeSeries are empty on creation", "[Core]") {

	// Arrange

	// Act
	HistoricData sut;

	// Assert
	REQUIRE(sut.dailyData().size() == 0);
}


TEST_CASE("HistoricData adds data", "[Core]") {

	// Arrange
	auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(31));
	double price = 250;
	HistoricData sut;

	// Act
	sut.addData(date, price, TimeFrame::DAILY);

	// Assert
	REQUIRE(sut.dailyData().size() == 1);
}


TEST_CASE("Historic updates its data.", "[Core]") {

	// Arrange
	auto dates = createDates(5);
	auto prices = createPrices(5);

	TimeSeries ts{ dates,prices };
	HistoricData sut;

	// Act
	sut.updateData(ts, TimeFrame::DAILY);

	// Assert
	REQUIRE(sut.dailyData().size() == 5);
}

// ----- LatestPrice Tests -----

TEST_CASE("LatestPrice updates its price", "[Core]") {

	// Arrange

	auto date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(30));
	auto new_date = std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(31));
	LatestPrice sut{ 250.0, date };

	// Act

	sut.updatePrice(500, new_date);

	// Assert

	REQUIRE(sut.price() == 500);
	REQUIRE(sut.timestamp() == std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(31)));
}