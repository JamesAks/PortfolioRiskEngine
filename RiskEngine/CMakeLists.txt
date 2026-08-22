set(RE_SOURCES
    
    src/Solvers/GMVPLagrangianSolver.cpp
    src/Solvers/MVLagrangianSolver.cpp
    src/CovarianceMatrix.cpp
    src/EfficientFrontier.cpp
    src/Objectives.cpp
    src/RiskEngine.cpp
    src/Solver.cpp
)


set(RE_HEADERS
    
    include/Solvers/GMVPLagrangianSolver.hpp
    include/Solvers/MVLagrangianSolver.hpp
    include/CovarianceMatrix.hpp
    include/EfficientFrontier.hpp
    include/Objectives.hpp
    include/Optimizer.hpp
    include/RiskEngine.hpp
    include/Solver.hpp
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

