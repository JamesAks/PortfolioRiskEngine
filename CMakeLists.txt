cmake_minimum_required(VERSION 3.20)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

Project(PortfolioRiskEngine

    VERSION 0.0.0
)

include(FetchContent)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(CURL CONFIG REQUIRED)

set(CMAKE_PREFIX_PATH $ENV{QTDIR})
find_package(Qt6 REQUIRED )
find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Gui)
find_package(Qt6 REQUIRED COMPONENTS Quick)
find_package(Qt6 REQUIRED COMPONENTS Graphs)
find_package(Qt6 REQUIRED COMPONENTS QuickWidgets)
find_package(Qt6 REQUIRED COMPONENTS Widgets)


FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.8.0

)

FetchContent_MakeAvailable(Catch2)

add_subdirectory(Apps)
add_subdirectory(Assets)
add_subdirectory(Core)
add_subdirectory(DataLoader)
add_subdirectory(MarketDataServices)
add_subdirectory(PricingEngines)
add_subdirectory(QuantitativeLibrary)
add_subdirectory(RiskEngine)
add_subdirectory(Tests)

enable_testing()

