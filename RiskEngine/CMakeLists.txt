# CMakeList.txt : CMake project for RiskEngine, include source and define
# project specific logic here.



# cmake_minimum_required (VERSION 3.8)
# project ("PortfolioRiskEngine")


set(RE_SOURCES
    
    src/RiskEngine.cpp
    src/PortfolioManager.cpp
    src/CovarianceMatrix.cpp
)


set(RE_HEADERS

    include/RiskEngine.hpp
    include/PortfolioManager.hpp
    include/CovarianceMatrix.hpp
)

add_library(RiskEngine STATIC ${RE_SOURCES} ${RE_HEADERS})
target_compile_features(RiskEngine PRIVATE cxx_std_20)


target_include_directories(RiskEngine 
    
    PUBLIC
        "C:/libs/C++/eigen-5.0.0" 
        include
)

target_link_libraries(RiskEngine 

    PUBLIC
    Core
    MarketDataServices
)

