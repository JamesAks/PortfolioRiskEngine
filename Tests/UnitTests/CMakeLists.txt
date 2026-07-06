
set(UTEST_SOURCES

    Assets/StockTest.cpp
	Core/HistoricDataTest.cpp
	Core/TimeSeriesTest.cpp
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


