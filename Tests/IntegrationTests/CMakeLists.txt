
add_executable(ITests)

target_link_libraries(ITests

	PRIVATE 
		QuantitativeLibrary
		Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(ITests)


