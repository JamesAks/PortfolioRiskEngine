add_executable(DataLoader main.cpp)

target_link_libraries(DataLoader 
	
	PUBLIC
		MarketDataServices
)
