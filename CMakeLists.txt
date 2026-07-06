

cmake_minimum_required(VERSION 3.20)

Project(PortfolioRiskEngine)

include(FetchContent)

find_package(CURL CONFIG REQUIRED)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0

)

FetchContent_MakeAvailable(Catch2)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Apps)
add_subdirectory(Assets)
add_subdirectory(Core)
add_subdirectory(DataLoader)
add_subdirectory(MarketDataServices)
add_subdirectory(PricingEngine)
add_subdirectory(QuantitativeLibrary)
add_subdirectory(RiskEngine)
add_subdirectory(Tests)

enable_testing()

