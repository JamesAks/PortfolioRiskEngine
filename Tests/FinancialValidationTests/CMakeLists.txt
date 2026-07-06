
add_executable(FVTests)

target_link_libraries(FVTests

	PRIVATE 
		QuantitativeLibrary
		Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(FVTests)


