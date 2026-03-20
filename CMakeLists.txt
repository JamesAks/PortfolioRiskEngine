cmake_minimum_required(VERSION 3.10.0)
project(PortfolioRiskEngine VERSION 0.1.0 LANGUAGES C CXX)


add_executable(PortfolioRiskEngine test.cpp src/TimeSeries.cpp src/Asset.cpp src/Portfolio.cpp)
target_include_directories(PortfolioRiskEngine PUBLIC ${CMAKE_SOURCE_DIR}/includes)
target_compile_features(PortfolioRiskEngine PUBLIC cxx_std_20)
add_subdirectory(src)
add_subdirectory(tests)

include(CTest)
enable_testing()

