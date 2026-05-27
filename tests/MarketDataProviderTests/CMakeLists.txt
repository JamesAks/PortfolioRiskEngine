add_executable(MDProviderTests main.cpp)

target_link_libraries(MDProviderTests

	PRIVATE
		MarketDataServices
)