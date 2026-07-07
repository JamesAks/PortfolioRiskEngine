set (FVTESTS_SOURCE

	Core/TimeSeriesFV.cpp

)

add_executable(FVTests ${FVTESTS_SOURCE} )

target_link_libraries(FVTests

	PRIVATE 
		QuantitativeLibrary
		Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(FVTests)


