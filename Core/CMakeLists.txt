

set(CORE_SOURCES
    
    src/Asset.cpp
    src/Core.cpp
    src/Logger.cpp
    src/Observing.cpp
    src/Portfolio.cpp
    src/Position.cpp
    src/Statistics.cpp
    src/MarketData.cpp
    src/TimeSeries.cpp
)


set(CORE_HEADERS

    include/Asset.hpp
    include/Core.hpp
    include/Logger.hpp
    include/Observing.hpp
    include/Portfolio.hpp
    include/Position.hpp
    include/Statistics.hpp
    include/MarketData.hpp
    include/TimeSeries.hpp
)

add_library(Core STATIC ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(Core 

	PUBLIC
		include
)
