set(PORT_APP_SOURCE
    
    src/AddPositionDialog.cpp
    src/AddPositionDialog.ui
    src/CreatePortfolioDialog.cpp
    src/CreatePortfolioDialog.ui
    src/EfficientFrontierGraph.cpp
    src/MainWindow.cpp
    src/MainWindow.ui
    src/PortfolioManager.cpp
    src/RemoveFromListDialog.cpp
    src/RemoveFromListDialog.ui
)

set(PORT_APP_HEADER
    
    include/AddPositionDialog.hpp
    include/CreatePortfolioDialog.hpp
    include/EfficientFrontierGraph.hpp
    include/MainWindow.hpp
    include/PortfolioManager.hpp
    include/RemoveFromListDialog.hpp
)

find_package(Qt6 REQUIRED)

add_executable(PortfolioApp 

    main.cpp 
    ${PORT_APP_HEADER}
    ${PORT_APP_SOURCE}
)

target_link_libraries(PortfolioApp 
    
    PRIVATE
    QuantitativeLibrary
    Qt::Core
    Qt::Gui
    Qt::Quick
    Qt::Graphs
    Qt::QuickWidgets
    Qt::Widgets
)

target_include_directories(PortfolioApp

	PUBLIC
		include
        src
)