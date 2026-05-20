

cmake_minimum_required(VERSION 3.20)

Project(PortfolioRiskEngine)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(Apps)
add_subdirectory(RiskEngine)
add_subdirectory(Core)
add_subdirectory(MarketDataStore)
add_subdirectory(PricingEngine)
add_subdirectory(QuantitativeLibrary)

add_subdirectory(tests)