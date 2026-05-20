add_library(MarketDataStore src/MarketDataStore.cpp include/MarketDataStore.hpp)

target_include_directories(MarketDataStore

	PUBLIC
		$(SOURCE_DIR)/include)