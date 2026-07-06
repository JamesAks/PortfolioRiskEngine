set(RE_SOURCES
    
    src/CovarianceMatrix.cpp
    src/RiskEngine.cpp

)

set(RE_HEADERS

    include/CovarianceMatrix.hpp
    include/RiskEngine.hpp
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

