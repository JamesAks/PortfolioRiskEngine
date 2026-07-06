
add_executable(E2ETests)

target_link_libraries(E2ETests

	PRIVATE 
		QuantitativeLibrary
		Catch2::Catch2WithMain
)

include(CTest)
include(Catch)

catch_discover_tests(E2ETests)

