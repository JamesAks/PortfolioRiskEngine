# CMakeList.txt : CMake project for RiskEngine, include source and define
# project specific logic here.



# cmake_minimum_required (VERSION 3.8)
# project ("PortfolioRiskEngine")


set(RE_SOURCES
    
    src/RiskEngine.cpp
    src/Asset.cpp
    src/Portfolio.cpp
    src/PortfolioManager.cpp
    src/RiskEngine.cpp 
    src/Statistics.cpp
    src/CovarianceMatrix.cpp
    src/Adapter.cpp
    src/MarketDataManager.cpp
    src/Logger.cpp
    src/Position.cpp
)


set(RE_HEADERS

    includes/RiskEngine.hpp
    includes/Asset.hpp
    includes/Portfolio.hpp
    includes/PortfolioManager.hpp
    includes/RiskEngine.hpp 
    includes/Statistic.hpp
    includes/CovarianceMatrix.hpp
    includes/Adapter.hpp
    includes/MarketDataManager.hpp
    includes/Logger.hpp
    includes/Position.hpp
)

include(FetchContent)

find_package(CURL CONFIG REQUIRED)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)
add_library(RiskEngine STATIC ${RE_SOURCES} ${RE_HEADERS})
target_compile_features(RiskEngine PRIVATE cxx_std_20)


target_include_directories(RiskEngine 
    
    PUBLIC
        "C:/libs/C++/eigen-5.0.0" 
        includes
)


target_link_libraries(RiskEngine PUBLIC 

    nlohmann_json::nlohmann_json 
    CURL::libcurl
)


