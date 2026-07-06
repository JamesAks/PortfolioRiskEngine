
set(MDS_SOURCES
    
	src/AlphaVantageProvider.cpp
	src/GenericDataStore.cpp
	src/CSVDataProvider.cpp
)

set(MDS_HEADERS
	
	include/GenericDataStore.hpp
	include/MarketDataStore.hpp
	include/MarketDataProvider.hpp
	include/AlphaVantageProvider.hpp
	include/CSVDataProvider.hpp


)

add_library(MarketDataServices ${MDS_SOURCES} ${MDS_HEADERS}) 

include(FetchContent)

find_package(CURL CONFIG REQUIRED)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

target_link_libraries(MarketDataServices

	PUBLIC
		Core
		nlohmann_json::nlohmann_json 
		CURL::libcurl
)

target_include_directories(MarketDataServices

	PUBLIC
		include
)