
set(UTEST_SOURCES

	Core/MarketDataTest.cpp
	Core/PortfolioTest.cpp
	Core/TimeSeriesTest.cpp

	MarketDataServices/AlphaVantageProviderTest.cpp
	MarketDataServices/GenericDataStoreTest.cpp

	RiskEngine/OptimizerTest.cpp

)
	

add_executable(UTests ${UTEST_SOURCES})

target_link_libraries(UTests

	PRIVATE 
		QuantitativeLibrary
		Catch2::Catch2WithMain
)



include(CTest)
include(Catch)

catch_discover_tests(UTests)


