
add_executable(PortfolioApp
    main.cpp
)

target_link_libraries(PortfolioApp 
    
    PRIVATE
    RiskEngine
)