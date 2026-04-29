# CMakeList.txt : CMake project for RiskEngine, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)
project ("PortfolioRiskEngine")

include(FetchContent)

find_package(CURL CONFIG REQUIRED)

FetchContent_Declare(
    json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)


# Add source to this project's executable.
add_executable (PortfolioRiskEngine "../test.cpp" "src/RiskEngine.cpp" "include/RiskEngine.hpp" "src/Asset.cpp"  "include/PortfolioManager.hpp"  "src/Portfolio.cpp"  "src/PortfolioManager.cpp"  "src/RiskEngine.cpp" "src/Statistics.cpp" "include/CovarianceMatrix.hpp" "src/CovarinceMatrix.cpp" "src/Adapter.cpp" "include/Adapter.hpp")

target_compile_features(PortfolioRiskEngine PRIVATE cxx_std_20)
target_include_directories(PortfolioRiskEngine PRIVATE "C:/libs/C++/eigen-5.0.0" )
target_link_libraries(PortfolioRiskEngine PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(PortfolioRiskEngine PRIVATE CURL::libcurl)

# TODO: Add tests and install targets if needed.
