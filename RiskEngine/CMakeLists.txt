# CMakeList.txt : CMake project for RiskEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("PortfolioRiskEngine")


# Add source to this project's executable.
add_executable (PortfolioRiskEngine "../test.cpp" "src/RiskEngine.cpp" "include/RiskEngine.hpp" "src/Asset.cpp"  "include/PortfolioManager.hpp"  "src/Portfolio.cpp"  "src/PortfolioManager.cpp"  "src/RiskEngine.cpp" "src/Statistics.cpp")

target_compile_features(PortfolioRiskEngine PRIVATE cxx_std_20)

# TODO: Add tests and install targets if needed.
