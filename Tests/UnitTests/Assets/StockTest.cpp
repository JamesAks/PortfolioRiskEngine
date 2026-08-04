#include <catch2/catch_test_macros.hpp>

#include "MarketData.hpp"
#include "Equities/Stock.hpp"
#include "TimeSeries.hpp"



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


// ----- Tests -----

TEST_CASE("Stock can return its NPV (latest price)", "[Core]") {

	// Arrange
	auto dates = createDates(5);
	auto prices = createPrices(5);

	TimeSeries test_series{ dates, prices };
	Stock sut{

		"TestStock",
		std::make_shared<LatestPrice>(40, std::chrono::year_month_day(std::chrono::year(2020), std::chrono::January, std::chrono::day(1))),
		std::make_shared<HistoricData>(test_series,test_series,test_series)
	};

	// Act

	sut.calculateNPV();

	// Assert
	REQUIRE(sut.NPV() == 40);
}


